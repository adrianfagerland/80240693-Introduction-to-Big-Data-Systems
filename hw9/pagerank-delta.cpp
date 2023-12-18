#include "core/graph.hpp"

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        fprintf(stderr, "usage: pagerank [path] [iterations] [memory budget in GB]\n");
        exit(-1);
    }
    std::string path = argv[1];
    int iterations = atoi(argv[2]);
    long memory_bytes = (argc >= 4) ? atol(argv[3]) * 1024l * 1024l * 1024l : 8l * 1024l * 1024l * 1024l;

    Graph graph(path);
    graph.set_memory_bytes(memory_bytes);
    BigVector<VertexId> degree(graph.path + "/degree", graph.vertices);
    BigVector<float> pagerank(graph.path + "/pagerank", graph.vertices);
    BigVector<float> sum(graph.path + "/sum", graph.vertices);
    BigVector<float> delta(graph.path + "/delta", graph.vertices);

    long vertex_data_bytes = (long)graph.vertices * (sizeof(VertexId) + sizeof(float) + sizeof(float) + sizeof(float));
    graph.set_vertex_data_bytes(vertex_data_bytes);

    double begin_time = get_time();

    degree.fill(0);
    graph.stream_edges<VertexId>(
        [&](Edge &e)
        {
            write_add(&degree[e.source], 1);
            return 0;
        },
        nullptr, 0, 0);
    printf("degree calculation used %.2f seconds\n", get_time() - begin_time);
    fflush(stdout);

    graph.hint(pagerank, sum, delta);

    graph.stream_vertices<VertexId>(
        [&](VertexId i)
        {
            pagerank[i] = 1.f / degree[i];
            sum[i] = 0;
            delta[i] = 0;
            return 0;
        },
        nullptr, 0,
        [&](std::pair<VertexId, VertexId> vid_range)
        {
            pagerank.load(vid_range.first, vid_range.second);
            sum.load(vid_range.first, vid_range.second);
            delta.load(vid_range.first, vid_range.second);
        },
        [&](std::pair<VertexId, VertexId> vid_range)
        {
            pagerank.save();
            sum.save();
            delta.save();
        });

    float propagation_threshold = 0.001f;

    for (int iter = 0; iter < iterations; iter++)
    {
        graph.hint(pagerank, delta);

        graph.stream_edges<VertexId>(
            [&](Edge &e)
            {
                float contribution = pagerank[e.source] * 0.85f / degree[e.source];
                write_add(&delta[e.target], contribution);
                return 0;
            },
            nullptr, 0, 1,
            [&](std::pair<VertexId, VertexId> source_vid_range)
            {
                pagerank.lock(source_vid_range.first, source_vid_range.second);
                delta.lock(source_vid_range.first, source_vid_range.second);
            },
            [&](std::pair<VertexId, VertexId> source_vid_range)
            {
                pagerank.unlock(source_vid_range.first, source_vid_range.second);
                delta.unlock(source_vid_range.first, source_vid_range.second);
            });

        graph.hint(pagerank, delta);

        graph.stream_vertices<float>(
            [&](VertexId i)
            {
                float new_delta = 0.15f + 0.85f * delta[i];
                if (delta[i] / pagerank[i] > propagation_threshold)
                {
                    pagerank[i] += delta[i];
                    delta[i] = 0;
                }
                delta[i] = new_delta;
                return 0;
            },
            nullptr, 0,
            [&](std::pair<VertexId, VertexId> vid_range)
            {
                pagerank.load(vid_range.first, vid_range.second);
                delta.load(vid_range.first, vid_range.second);
            },
            [&](std::pair<VertexId, VertexId> vid_range)
            {
                pagerank.save();
                delta.save();
            });
    }

    double end_time = get_time();
    printf("%d iterations of pagerank took %.2f seconds\n", iterations, end_time - begin_time);
}

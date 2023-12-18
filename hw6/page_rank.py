from pyspark import SparkConf, SparkContext
import sys
import time


def pagerank_iter(edge_weights, scores, damping, num_nodes):
    score_new = edge_weights.join(scores).flatMap(
        lambda node: [(n, node[1][1] / len(node[1][0])) for n in node[1][0]]
    ).reduceByKey(lambda a, b: a + b).mapValues(
        lambda x: damping * x + (1.0 - damping) / num_nodes
    ).cache()
    return score_new


if __name__ == '__main__':
    conf = SparkConf()
    sc = SparkContext(conf=conf)
    sc.setLogLevel("WARN")
    lines = sc.textFile(sys.argv[1])

    first = time.time()

    edges = lines.map(lambda line: tuple(map(int, line.split())))
    edges = edges.distinct().groupByKey().cache()

    num_nodes = edges.count()
    initial_score = edges.map(lambda node: (
        node[0], 1 / num_nodes)).partitionBy(num_nodes).cache()

    damping = 0.8
    MAXITER = 100
    converged = False
    iter = 0

    while not converged and iter < MAXITER:
        print(f"starting iteration {iter}")
        next_score = pagerank_iter(edges, initial_score, damping, num_nodes)
        global_diff = initial_score.join(next_score).map(
            lambda node: abs(node[1][0] - node[1][1])
        ).sum()
        print(f"global_diff: {global_diff}")
        # converged = global_diff < 0.000001
        initial_score = next_score
        iter += 1

    highest = initial_score.takeOrdered(5, key=lambda x: -x[1])
    print("5 highest:", highest)

    last = time.time()

    print("Total program time: %.2f seconds" % (last - first))

    sc.stop()

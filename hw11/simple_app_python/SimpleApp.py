from __future__ import print_function

import sys

from pyspark import RDD, SparkContext
from pyspark.streaming import StreamingContext


def get_top_100_words(current_counts: RDD, time):
    global history
    if history is None:
        history = current_counts
    else:
        history = current_counts.union(history).reduceByKey(lambda a, b: a + b)

    top100 = history.takeOrdered(100, key=lambda x: -x[1])

    output_path = f"top_100_words_{time}.txt"
    with open(output_path, "w") as file:
        for word, count in top100:
            print(f"{word} {count}")
            file.write(f"{word} {count}\n")
    print()


sc = SparkContext(appName="Py_HDFSWordCount")
ssc = StreamingContext(sc, 60)

lines = ssc.textFileStream("hdfs://intro00:9000/user/bds23-ghcstu10/stream")

word_counts = lines.flatMap(lambda line: line.split(" ")).map(
    lambda x: (x, 1)).reduceByKey(lambda a, b: a + b)

history = None

word_counts.foreachRDD(lambda time, rdd: get_top_100_words(rdd, time))

ssc.start()
ssc.awaitTermination()

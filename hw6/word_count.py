import re
import sys
from pyspark import SparkConf, SparkContext
import time


if __name__ == '__main__':
    conf = SparkConf()
    sc = SparkContext(conf=conf)
    lines = sc.textFile(sys.argv[1])

    first = time.time()

    # Students: Implement Word Count!
    counts = lines.flatMap(lambda line: line.split()) \
                  .map(lambda word: (word, 1)) \
                  .reduceByKey(lambda a, b: a + b)

    output = counts.takeOrdered(10, key=lambda x: -x[1])
    for x in output:
        print(x)

    last = time.time()

    print("Total program time: %.2f seconds" % (last - first))
    sc.stop()

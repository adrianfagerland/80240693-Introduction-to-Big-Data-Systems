# Homework 4

## Part 1

### 1.1

From the paper: It asks the each chunkserver about its chunks at master startup and whenever a chunkserver joins the cluster. In other words, it polls the chunkservers and gets the data from them.

### 1.2

Not needing to persistently store the chunk locations means that the master does not need to handle updates in a persistant record, allowing for greater flexibility of chunkserver addition and removals, not having to worry about stale data. The necessary overhead is also reduced. Also, the master does not need to worry about keeping the data persistent, as it can easily be recovered in the case of a crash. Last but not least, the entire system becomes more scalable from the added flexibility, as it is easier for the master to know when to update the chunk locations, not having to worry about updates in the persistent data.

## Part 2

### 2.1

The way I understand the question, we need to transfer the total amount of data that was in the failed node to the network by accessing its replicas. We are assuming the failed node was full of data. We also asume that the chunk size is 64MB, meaning that there are many disks over many servers that hold a replica of the data that was on the failed node. We have 100TB of data that we need to make a new replication of over a network with a bandwidth of $1\text{Gbps} = 125 \text{MB/s}$. We also assume this bandwidth is between each server. In the best case, 499 servers hold the replicas of the failed node, and the information can be streamed to the other 499 servers to create a new replica. The bottleneck in this case is not the W/R speed of the disk, and several disks can hold chunks of replicas of the failed node, so we disregard the W/R speed.
$$
\text{Time to transfer 100 TB} = \frac{100\text{TB} \cdot 1024\text{GB}/\text{TB} \cdot 1024\text{MB}/\text{GB}}{125\text{MB}/\text{s} \cdot 499} \approx 1681 \text{s}
$$
Given that we make a lot of assumptions, the minimum time required to recovery a node failure in this case is therefore $1681 \text{s}$.

### 2.2

We use the same method as the first question, this time with a bandwidth of  $100\text{Mbps} = 12.5\text{MB}/\text{s}$.
$$
\text{Time to transfer 100 TB} = \frac{100\text{TB} \cdot 1024\text{GB}/\text{TB} \cdot 1024\text{MB}/\text{GB}}{12.5\text{MB}/\text{s} \cdot 499} \approx 16811 \text{s}
$$

The minimum time required to recovery a node failure in this case is therefore $16810 \text{s}$.

### 2.3

The amount of hours in a year is

$$
365.25 \text{day} \cdot  24\text{hours}/\text{day} = 8766 \text{hours}.
$$

On average, a node fails every 10 000 hours according to the problem description. This means that after 10 000 hours, $0.5\cdot \text{amount of nodes}$ failures have occured. Meaning after $87.66\%$ of that time, $87.66\% \cdot  50\% = 43.83\%$, $1000 \cdot  0.4383 \approx 438$ failures have occured.

If 438 failures occur in a year, that means that there is an error every $\frac{8766 \text{hours per year}}{438 \text{failure per year}} \approx 20 \text{hours}/\text{failure}$.

### 2.4

Given that it takes less time to recover from a node failure than the expected time between failures, it would be possible to only have two replicas in this case. However, chunks will likely be lost if there are two failures within 4 hours, so I would recommend at least 3 replicas.

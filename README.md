Mapper
======

Local Map / Reduce framework

Recently we tried Apache Hadoop for some data processing.

We installed it on two nodes cluster (16 map / 8 reduce capacity) and we did some tests.

For our surprise, I found Hadoop to be very slow for processing we doing. Our highly optimized PHP scripts worked about 100 times faster than our Hadoop installation. We estimated that we need at least 5-6 node cluster to have same performance that we currently use.

I believe, some other people have exactly the same problem, this is why I did local non scalable remake of Hadoop streaming engine.

Because we needed more mapping than reducing, my version includes only mapping part.


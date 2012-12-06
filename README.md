Mapper
======

Local Map / Reduce framework

Recently we tried Apache Hadoop for some data processing.

We installed it on two nodes cluster (16 map / 8 reduce capacity) and we did some tests.

For our surprise, I found Hadoop to be very slow for processing we doing. 
Our highly optimized PHP scripts worked about 100 times faster than our Hadoop installation. 
We estimated that we need at least 5-6 node cluster to have same performance that we currently use.

I believe, some other people have exactly the same problem, 
this is why I did local non scalable remake of Hadoop streaming engine.

Because we needed more mapping than reducing, my version includes only the mapping part.

Program can be start in following way:

  ./mapper [input_file] [mapper_program] [[optional number of threads]]

e.g.

  ./mapper /data/my_big_file.txt ./demo_map.php 64 > output 2> output.log &

You can start the program with standard input as input file.
This allows you to get data from different sources such MySQL or MongoDB:

  ./get_data_from_db.php | ./mapper - ./demo_map.php 64 > output 2> output.log &
  
or you can feed compressed data:

  zcat /data/my_big_file.txt.gz | ./mapper - ./demo_map.php 64 > output 2> output.log &

If you need, you still can use reducer:

  ./get_data_from_db.php | ./mapper - ./demo_map.php 64 2> output.log | ./reducer.php > output

or if you need sorted input for the reducer:

  ./get_data_from_db.php | ./mapper - ./demo_map.php 64 2> output.log | sort | ./reducer.php > output



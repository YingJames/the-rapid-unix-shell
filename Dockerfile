FROM ubuntu:22.04
  
RUN apt-get update \
	&& apt-get install gcc build-essential \
	make valgrind vim git gh \
	curl unzip  -y

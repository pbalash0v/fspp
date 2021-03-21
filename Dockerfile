FROM debian:buster-backports
RUN apt update && apt-get install --no-install-recommends -y -t buster-backports \
	cmake \
	python-dev \
	build-essential \
	autoconf \
	automake \
	libtool \
	libtool-bin \	
	pkg-config \
	libboost1.74-dev \
	libssl-dev \
	libsqlite3-dev \
	zlib1g-dev \
	libspeexdsp-dev \
	libspeex-dev \
	libtpl-dev \
	libpcre3-dev \
	libcurl4-openssl-dev \
	uuid-dev \
	git \
	ca-certificates \
	libtiff5-dev \
	libsndfile-dev
WORKDIR /src
CMD mkdir -p build && \
	cd build && \
	cmake -DCMAKE_BUILD_TYPE=Release ../ && \
	cmake --build .

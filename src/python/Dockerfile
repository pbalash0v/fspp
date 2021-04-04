FROM debian:buster-slim
COPY build/fspp*deb /src/
WORKDIR /src
RUN apt update && apt install -y --no-install-recommends \
	python-minimal \
	/src/fspp* 
COPY build/out/bin/version.py /opt/fspp/bin/
WORKDIR /opt/fspp/bin
CMD PYTHONPATH=$PYTHONPATH:/opt/fspp/lib ./version.py


FROM debian:buster-slim
WORKDIR /src
RUN apt update && apt install -y --no-install-recommends python-minimal
COPY build/fspp*deb /src/
RUN apt install -y --no-install-recommends	/src/fspp*
COPY build/out/bin/version.py /opt/fspp/bin/
WORKDIR /opt/fspp/bin
CMD PYTHONPATH=$PYTHONPATH:/opt/fspp/lib ./version.py


# fspp

docker image build -t fspp .
docker run --mount src=`pwd`,target=/src,type=bind fspp

# fspp

Build docker image with necessary build tools:
```shell
docker image build -t fspp_build .
```
Run library build:
```shell
docker run --mount src="$(pwd)",target=/src,type=bind fspp_build
```


Build docker image for simple example app:
```shell
docker image build -t fspp_app -f src/python/Dockerfile .
```
Run app:
```shell
docker run --rm --net=host fspp_app
```

# fspp

Every command should be run in git source folder.

Build docker image with necessary build tools:
```shell
docker image build -t fspp_build .
```
Run library build (will create a `build/` folder and produce simple deb package there):
```shell
docker run --mount src="$(pwd)",target=/src,type=bind fspp_build
```


Build docker image for simple example app (will use deb package produced earlier):
```shell
docker image build -t fspp_app -f src/python/Dockerfile .
```
Run app:
```shell
docker run --rm --net=host fspp_app
```

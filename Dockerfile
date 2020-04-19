FROM alpine:latest
RUN apk add --no-cache openssl-dev boost-dev curl-dev cmake ninja bash gcc g++ gtest-dev rpm

WORKDIR /service/
COPY 3rdparty 3rdparty
COPY CMakeLists.txt CMakeLists.txt
COPY build.sh build.sh
COPY .env .env
COPY test test
COPY src src
RUN /bin/bash build.sh -b
CMD ["/bin/bash", "build.sh", "-rb"]
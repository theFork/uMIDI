FROM alpine:3.19

RUN apk update && apk add avr-libc doxygen make perl --no-cache

RUN mkdir /src
COPY ./doc /src/doc
COPY ./software /src/software
COPY ./docker/entrypoint.sh /

ENTRYPOINT ["/entrypoint.sh"]

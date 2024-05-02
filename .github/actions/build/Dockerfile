FROM alpine:latest

RUN apk update && apk add avr-libc doxygen make perl --no-cache

COPY ../../software /

ENTRYPOINT ["/entrypoint.sh"]

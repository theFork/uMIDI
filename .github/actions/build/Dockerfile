FROM alpine:3.19

RUN apk update && apk add avr-libc doxygen make perl --no-cache

COPY ./entrypoint.sh /
ENTRYPOINT ["/entrypoint.sh"]

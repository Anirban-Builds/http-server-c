FROM gcc:latest

WORKDIR /app

COPY server.c .
COPY helper.c .
COPY include/ ./include/

RUN gcc server.c helper.c -I./include -o server

ENV PORT=7860

EXPOSE 7860

CMD ["./server"]
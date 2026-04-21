FROM gcc:latest

WORKDIR /app

COPY server.c .

RUN gcc server.c -o server

ENV PORT=7860

EXPOSE 7860

CMD ["./server"]
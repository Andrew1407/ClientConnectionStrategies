import net from 'node:net';

export default function createSever(port, host, onData) {
  const server = net.createServer(async socket => {
    socket.on('data', data => {
      const result = onData(JSON.parse(data));
      socket.write(JSON.stringify({ result }));
    });
  });
  server.listen(port, host);
  return server;
};

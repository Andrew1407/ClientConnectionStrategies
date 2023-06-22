import { WebSocketServer } from 'ws';

export default function createSever(port, host, onData) {
  const server = new WebSocketServer({ port, host });
  server.on('connection', socket => {
    socket.on('message', data => {
    const result = onData(JSON.parse(data));
    socket.send(JSON.stringify({ result }));
    });
  });
  return server;
};

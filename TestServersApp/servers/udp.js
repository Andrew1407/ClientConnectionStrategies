import dgram from 'dgram';

export default function createSever(port, host, onData) {
  const server = dgram.createSocket({ type: 'udp4' });
  const heartbeatCommands = ['heartbeat', 'healthcheck', 'heart_beat', 'health_check'];
  server.on('message', (data, remote) => {
    const heartbeatCheck = heartbeatCommands.includes(data.toString().toLowerCase());
    if (heartbeatCheck) {
      const alveMessage = 'OK';
      server.send(alveMessage, remote.port, remote.address);
    } else {
      const result = onData(JSON.parse(data));
      server.send(JSON.stringify({ result }), remote.port, remote.address);
    }
  });
  server.bind(port, host);
  return server;
};

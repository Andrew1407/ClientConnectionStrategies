import http from 'http';
import url from 'url';

export default function createSever(port, host, onData) {
  const server = http.createServer((req, res) => {
    if (req.method === 'GET') {
      const { query } = url.parse(req.url, true);
      query.denominations = query.denominations.split(',').map(Number);
      query.amount = Number(query.amount);
      const result = onData(query);
      res.end(JSON.stringify({ result }));
    } else if (req.method === 'POST') {
      let chunks = '';
      req.on('data', ch => void(chunks += ch));
      req.on('end', () => {
        const result = onData(JSON.parse(chunks));
        res.end(JSON.stringify({ result }));
      });
    } else {
      res.statusCode = 404;
      res.end();
    }
  });
  server.listen(port, host);
  return server;
};

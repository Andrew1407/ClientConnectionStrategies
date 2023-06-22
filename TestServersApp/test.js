import test from 'node:test';
import assert from 'node:assert';
import { WebSocket } from 'ws';
import { Socket } from 'net';
import dgram from 'dgram';
import env from './env.json' assert { type: 'json' };

const { host, port } = env;

const expected = {
  http: 3,
  ws: 0,
  udp: -1,
  tcp: 1,
};

const params = {
  http: { denominations: [1, 2, 5], amount: 11, result: undefined, },
  ws: { denominations: [1, 2, 5], amount: 0, result: undefined, },
  udp: { denominations: [3, 2, 5], amount: 1, result: undefined, },
  tcp: { denominations: [3, 2, 5], amount: 3, result: undefined, },
};

const fetchers = {
  // http: async ({ denominations, amount }, i) => {
  //   const url = `http://${host}:${port + i}/?denominations=${denominations}&amount=${amount}`;
  //   const res = await fetch(url, { method: 'GET' });
  //   const { result } = await res.json();
  //   return result;
  // },
  http : async (params, i) => {
    const url = `http://${host}:${port + i}/`;
    const body = JSON.stringify(params);
    const res = await fetch(url, { method: 'POST', body });
    const { result } = await res.json();
    return result;
  },
  ws: (params, i) => new Promise(res => {
    const client = new WebSocket(`ws://${host}:${port + i}/`);
    client.onmessage = e => res(JSON.parse(e.data).result);
    client.onopen = () => client.send(JSON.stringify(params), () => client.close());
  }),
  udp: (params, i) => new Promise(res => {
    const client = dgram.createSocket({ type: 'udp4' });
    client.on('message', data => (client.close(), res(JSON.parse(data).result)));
    client.send(JSON.stringify(params), port + i, host);
  }),
  tcp: (params, i) => new Promise(res =>{
    const client = new Socket();
    client.on('data', data => (client.destroy(), res(JSON.parse(data).result)));
    client.connect(port + i, host, () => client.write(JSON.stringify(params)));
  }),
};

test('clinets correct respnse message', async t => {
  await Promise.all(Object.entries(params).map(async ([ key, args ], i) => {
    if (key in fetchers) params[key].result = await fetchers[key](args, i);
  }));

  console.table(params);
  
  await Promise.all(Object.keys(params).map(key =>
    t.test(`${key} client result check`, () => assert.equal(expected[key], params[key].result))
  ));
});

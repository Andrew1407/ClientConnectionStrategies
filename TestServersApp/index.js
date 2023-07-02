import exchange from './exchange.js';
import env from './env.json' assert { type: 'json' };

const { host = 'localhost', port = 8080, labels = [] } = env;
const moduleLoaders = labels.map(c => import(`./servers/${c}.js`));
const loaded = await Promise.allSettled(moduleLoaders);
const modules = loaded.reduce((acc, m, i) => ({ ...acc, [labels[i]]: m }), {});
const factories = {};
for (const key in modules) {
  const { reason, value, status } = modules[key];
  if (status == 'fulfilled') factories[key] = value.default;
  else console.error(`Failed to load "${key}" module: ${reason}`);
}

const onData = ({ denominations, amount }) => exchange(denominations, amount);
const serverOptions = {};
const servers = Object.keys(factories).map((key, i) => {
  const addr = { host, port: port + i };
  serverOptions[key] = addr;
  return factories[key]( addr.port, addr.host, onData);
});

console.table(serverOptions);

const close = server => new Promise(r => server ? server.close(r) : r());
const shutdown = () => Promise.allSettled(servers.map(close))
  .finally(() => (console.log(), process.exit(0)));
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);

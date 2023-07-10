import exchange from './exchange.js';

/**
 * Searches min exchange by passed params object
 * @param {{ denominations: number[], amount: number }} params - available denominations and amount to exchange
 * @returns {number} min exchange
*/
export const onData = ({ denominations, amount }) => exchange(denominations, amount);

/**
 * Creates object of server factories by given labels
 * @param {string[]} labels - types of server protocols
 * @returns {Promise<Record<string, object>>} factory function collection
*/
export const getFatofies = async labels => {
  const moduleLoaders = labels.map(c => import(`./servers/${c}.js`));
  const loaded = await Promise.allSettled(moduleLoaders);
  const modules = loaded.reduce((acc, m, i) => ({ ...acc, [labels[i]]: m }), {});
  const factories = {};
  for (const key in modules) {
    const { reason, value, status } = modules[key];
    if (status == 'fulfilled') factories[key] = value.default;
    else console.error(`Failed to load "${key}" module: ${reason}`);
  }
  return factories;
};

/**
 * Creates servers collection by passed factories
 * @param {Record<string, object>} factories - factory function collection
 * @param {string} host - conection host
 * @param {number} port - connection port
 * @param {(params: { denominations: number[], amount: number }) => number} onData - data handler
 * @returns {Record<string, object>} server strategies collection
*/
export const createServers = (factories, host, port, onData) => {
  const serverOptions = {};
  const servers = Object.keys(factories).map((key, i) => {
    const addr = { host, port: port + i };
    serverOptions[key] = addr;
    return factories[key](addr.port, addr.host, onData);
  });
  console.table(serverOptions);
  return servers;
};

/**
 * Generates strategies (servers) by labels
 * @param {{
 *  host: string,
 *  port: number,
 *  labels: string[],
 *  onData: (params: { denominations: number[], amount: number }) => number
 * }} params - conection host & port, types of server protocols, data handler
 * @returns {Promise<{ servers: Record<string, object>, close: () => Promise<void> }>} server strategies collection and shutdown handler
*/
export const getStrategies = async ({ host, port, labels, onData }) => {
  const factories = await getFatofies(labels);
  const servers = createServers(factories, host, port, onData);
  const closeServer = server => new Promise(r => server ? server.close(r) : r());
  const close = () => Promise.allSettled(servers.map(closeServer));
  return { servers, close };
};

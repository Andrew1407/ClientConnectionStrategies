import { onData, getStrategies } from './strategies.js';
import env from './env.json' assert { type: 'json' };

const { host = 'localhost', port = 8080, labels = [] } = env;
const strategies = await getStrategies({ host, port, labels, onData });

const shutdown = () => strategies.close().finally(() => (console.log(), process.exit(0)));
process.on('SIGINT', shutdown);
process.on('SIGTERM', shutdown);

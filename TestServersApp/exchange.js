/**
 * Searches min exchange
 * @param {number[]} denominations - available denominations
 * @param {number} amount - amount to exchange
 * @returns {number} min exchange
*/
export default function exchange(denominations, amount) {
  const exchanges = Array(amount + 1).fill(Infinity);
  exchanges[0] = 0;

  for (const denomination of denominations)
    for (let j = denomination; j <= amount; ++j)
      exchanges[j] = Math.min(exchanges[j], exchanges[j - denomination] + 1);

  return exchanges[amount] === Infinity ? -1 : exchanges[amount];
};

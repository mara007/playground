#!/usr/bin/env python3
import logging

logger = logging.getLogger(__name__)

class Chart:
    def __init__(self, data: dict) -> None:
        self.data = data
        self.max_bar_chars = 150
        self.bar_chars = '🧙🦄🦴👾🌻'
        self.bar_chars = '🧙🦄🦴👾🕷🌻🌶'
        # self.bar_chars = '🍓'
        self.bar_char_last = 0
        self.padding = 4

    def __get_longest_key(self) -> int:
        longest = 0
        for k in self.data.keys():
            longest = max(longest, len(k))

        logger.debug(f'{longest=}')
        return longest

    def __sum_all_values(self) -> int:
        suma = sum(self.data.values())
        logger.debug(f'summ of all values = {suma}')
        return suma

    def __get_percent_bar(self, value, sum_of_all) -> str:
        percent = value / sum_of_all * 100
        bars = self.max_bar_chars * percent / 100
        result = self.bar_chars[self.bar_char_last % len(self.bar_chars)] * round(bars)
        self.bar_char_last += 1
        return result

    def __get_percent(self, value, sum_of_all) -> str:
        return f'{(value / sum_of_all * 100):.2f}%'


    def print_chart(self):
        longest_key = self.__get_longest_key()
        sum_of_all = self.__sum_all_values()

        print(f'{"class":>{longest_key + self.padding}}{" ":>{self.padding}}{"%":>{6}}{" ":>{self.padding}}{"percentual":<{longest_key + 2* self.padding}}')
        for key, value in self.data.items():
            percent = self.__get_percent(value, sum_of_all)
            bar = self.__get_percent_bar(value, sum_of_all)
            print(f'{key:>{longest_key + self.padding}}{" ":>{self.padding}}{percent:>{6}}{" ":>{self.padding}}{bar:<{longest_key + 2* self.padding}}')

        print(f'{"votes":>{longest_key + self.padding}}{" ":>{self.padding}}{sum_of_all:>{6}}')


if __name__ == '__main__':
    data = {
        # 30.8.2022
        'wotlk_fresh_thekal': {
            'druid': 155,
            'death knight': 143,
            'paladin': 246,
            'mage': 103,
            'hunter': 111,
            'shaman': 101,
            'priest': 104,
            'rogue': 131,
            'warrior': 130,
            'warlock': 108
        },
        # 22.8.2022
        'wotlk_fresh_disc': {
            'druid': 197,
            'death knight': 220,
            'paladin': 291,
            'mage': 129,
            'hunter': 134,
            'shaman': 152,
            'priest': 152,
            'rogue': 144,
            'warrior': 147,
            'warlock': 131
        },
        # 12.8.2022
        'wotlk_disc': {
            'druid': 270,
            'death knight': 500,
            'paladin': 457,
            'mage': 200,
            'hunter': 177,
            'shaman': 236,
            'priest': 237,
            'rogue': 184,
            'warrior': 211,
            'warlock': 232
        },
    }

    for name, dato in data.items():
        chart = Chart(data=dato)
        print(f'Discord data for {name}:')
        chart.print_chart()
        print()

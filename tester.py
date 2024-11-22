#!/usr/bin/env python3


class Wrap:
    def __init__(self, name:str) -> None:
        self.name = name
        pass

    def __del__(self) -> None:
        print(f'Wrap({self.name}).__del()__')

def test_wrap():
    wrap_global = Wrap('glob')
    if True:
        wrap_block = Wrap('block')

        try:
            wrap_try = Wrap('try')
            raise NameError('throwing')
        except NameError as e:
            print('cought..')


test_wrap()

# def fail_gen():
#     i = 1
#     while i < 10:
#         print('before yield')
#         yield i+1
#         print('after yield')
#         i += 1
#
#     print('after yield in fail_gen()')
#
# for i in fail_gen():
#     print(f'failgen: {i=}')

#!/usr/bin/env python3

def fail_gen():
    i = 1
    while i < 10:
        print('before yield')
        yield i+1
        print('after yield')
        i += 1

    print('after yield in fail_gen()')

for i in fail_gen():
    print(f'failgen: {i=}')
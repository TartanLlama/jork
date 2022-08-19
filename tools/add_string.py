import argparse

parser = argparse.ArgumentParser(description='Add a string to the global translation table.')
parser.add_argument('key', type=str, help='key to refer to in the code')
parser.add_argument('english_translation', type=str, help='the english translation of the string')
args = parser.parse_args()

lines = None
with open('source/strings.ixx','r') as header:
	lines = header.readlines()

lines.insert(-2, '        inline string_key {} {{ "{}" }};\n'.format(args.key, args.key))

with open('source/strings.ixx','w') as header:
	header.write(''.join(lines))

with open('source/strings.cpp','r') as impl:
	lines = impl.readlines()

lines.insert(-1, '    {{ strings::{}, "{}" }},\n'.format(args.key, args.english_translation))

with open('source/strings.cpp','w') as impl:
	impl.write(''.join(lines))
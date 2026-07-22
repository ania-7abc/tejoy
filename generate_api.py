import re
import os
import sys
import argparse
import subprocess
import tempfile
import shutil

def convert_links(markdown, src_path):
    current_dir = os.path.dirname(src_path)

    def repl(m):
        text = m.group(1)
        link = m.group(2)
        if '#' in link:
            base, anchor = link.split('#', 1)
            if base == '':
                return f'[{text}](#{anchor})'
        else:
            base, anchor = link, ''
        if '/' in base or base.startswith('http'):
            return m.group(0)
        if not base.endswith('.md'):
            return m.group(0)
        name = base[:-3]
        target_path = os.path.join(name.replace('-', os.sep), 'index.md')
        rel = os.path.relpath(target_path, start=current_dir)
        rel = rel.replace(os.sep, '/')
        return f'[{text}]({rel}#{anchor})'

    return re.sub(r'\[([^]]*)]\(([^)]+)\)', repl, markdown)

def main():
    parser = argparse.ArgumentParser(description='Generate API documentation from Doxygen XML using moxygen.')
    parser.add_argument('--doxyfile', default='Doxyfile', help='Path to Doxyfile (default: Doxyfile)')
    parser.add_argument('--output', default='docs/api', help='Output directory for API Markdown files (default: docs/api)')
    parser.add_argument('--doxygen', default='doxygen', help='Doxygen executable (default: doxygen)')
    parser.add_argument('--moxygen', default='moxygen', help='Moxygen executable (default: moxygen)')
    parser.add_argument('--keep-temp', action='store_true', help='Keep temporary directories for debugging')
    args = parser.parse_args()

    temp_dir = tempfile.mkdtemp(prefix='doxygen_moxygen_')
    xml_dir = os.path.join(temp_dir, 'xml')
    md_dir = os.path.join(temp_dir, 'md')
    os.makedirs(xml_dir, exist_ok=True)
    os.makedirs(md_dir, exist_ok=True)

    try:
        with open(args.doxyfile, 'r') as f:
            doxy_content = f.read()

        doxy_content += f'\nGENERATE_XML=YES\nOUTPUT_DIRECTORY="{temp_dir}"\n'

        subprocess.run(
            [args.doxygen, '-'],
            input=doxy_content,
            text=True,
            capture_output=True,
            check=True
        )

        subprocess.run(
            [args.moxygen, '-cHf', '-o', os.path.join(md_dir, '%s.md'), xml_dir],
            check=True,
            capture_output=True,
            text=True
        )

        for root, _, filenames in os.walk(md_dir):
            for filename in filenames:
                if not filename.endswith('.md'):
                    continue
                file_path = os.path.join(root, filename)
                with open(file_path, 'r', encoding='utf-8') as f:
                    content = f.read()

                if filename == 'api.md':
                    target_dir = ''
                else:
                    target_dir = filename[:-3].replace('-', os.sep)

                src_path = os.path.join(target_dir, 'index.md')
                new_content = convert_links(content, src_path)

                output_file = os.path.join(args.output, target_dir, 'index.md')
                os.makedirs(os.path.dirname(output_file), exist_ok=True)
                with open(output_file, 'w', encoding='utf-8') as f:
                    f.write(new_content)

    except Exception as e:
        print(f'Error: {e}', file=sys.stderr)
        sys.exit(1)
    finally:
        if not args.keep_temp:
            shutil.rmtree(temp_dir)

if __name__ == '__main__':
    main()

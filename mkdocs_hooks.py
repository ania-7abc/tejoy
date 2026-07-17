import logging
import os
import subprocess
import tempfile
from pathlib import Path
from mkdocs.config.defaults import MkDocsConfig
from mkdocs.structure.files import Files, File
from mkdocs.structure.pages import Page

log = logging.getLogger(__name__)

def on_files(files: Files, config: MkDocsConfig) -> Files:
    with tempfile.TemporaryDirectory(delete=False) as tmp_dir:
        xml_dir = os.path.join(tmp_dir, "xml")
        api_dir = os.path.join(tmp_dir, "api")
        os.makedirs(xml_dir, exist_ok=True)
        os.makedirs(api_dir, exist_ok=True)

        with open("Doxyfile", "r") as f:
            doxy_content = f.read()

        subprocess.run(
            ["doxygen", "-"],
            input=doxy_content + f"\nGENERATE_XML=YES\nOUTPUT_DIRECTORY=\"{tmp_dir}\"\n",
            text=True,
            capture_output=True,
            check=True
        )

        subprocess.run(
            ["moxygen", "-c", "-a", "-o", os.path.join(api_dir, "api-%s"), xml_dir],
            check=True,
            capture_output=True,
            text=True
        )

        for root, _, filenames in os.walk(api_dir):
            for filename in filenames:
                if filename.endswith(".md"):
                    file_path = os.path.join(root, filename)

                    with open(file_path, "r", encoding="utf-8") as f:
                        content = f.read()

                    files.append(File.generated(
                        config,
                        os.path.join(filename[:-3].replace("-", "/"), "index.md"),
                        content = content
                    ))

    return files


def on_page_markdown(markdown: str, page: Page, config: MkDocsConfig, files: Files) -> str:
    return markdown

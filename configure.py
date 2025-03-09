import os
import sys
import subprocess
import base64
import shutil
import json
import zipfile
import io
from urllib import request

class App:
    def __init__(self, args: list) -> None:
        self.exit_code = 0
        self.cwd = os.path.dirname(__file__) or os.getcwd()
        self.print_logo()
        self.miniz_path = os.path.join(self.cwd, 'ImageLib', 'miniz')
        self.sdl_path = os.path.join(self.cwd, 'SDL3')
        self.fetch_default_libs()

    def fetch_default_libs(self) -> None:
        print('[INFO] Fetching default libs...')
        if os.path.isdir(self.miniz_path):
            print('[INFO] Miniz found')
        else:
            print('[INFO] Miniz not found, fetching...')
            resp = self.request_json('https://api.github.com/repos/richgel999/miniz/releases/latest')
            assert len(resp['assets']) == 1
            zip_data = self.request_bin(resp['assets'][0]['browser_download_url'])
            zip_f = zipfile.ZipFile(io.BytesIO(zip_data))
            os.mkdir(self.miniz_path)
            zip_f.extractall(self.miniz_path)
        if os.path.isdir(self.sdl_path):
            print(f'[INFO] SDL3 found')
        else:
            print(f'[INFO] Fetching SDL3...')
            os.rename(self.download_sdl_lib('', '-VC.zip', 3), self.sdl_path)

    def download_sdl_lib(self, lib_postfix: str, asset_postfix: str, sdl_ver: int) -> str:
        rel_info = self.fetch_sdl_rel_info(lib_postfix, sdl_ver)
        for asset_info in rel_info['assets']:
            if asset_info['name'].endswith(asset_postfix):
                break
        else:
            assert False
        assert asset_info['name'].endswith(asset_postfix)
        zip_data = self.request_bin(asset_info['browser_download_url'])
        print(f'[INFO] Unzipping SDL{sdl_ver}{lib_postfix}...')
        zp = zipfile.ZipFile(io.BytesIO(zip_data))
        zp.extractall(self.cwd)
        return os.path.join(self.cwd, 'SDL' + str(sdl_ver) + lib_postfix + '-' + rel_info['name'])

    def fetch_sdl_rel_info(self, lib_postfix: str, sdl_ver: int) -> dict:
        resp = self.request_json(f'https://api.github.com/repos/libsdl-org/SDL{lib_postfix}/releases')
        assert len(resp) >= 1
        for rel_info in resp:
            if rel_info['name'].startswith(str(sdl_ver) + '.'):
                break
        else:
            assert False
        assert len(rel_info['assets']) >= 1
        return rel_info

    @staticmethod
    def print_logo() -> None:
        data = b'ICAgICAgIC4sICAgICAgICAgICAgICAgICAgICAsLi0uICAgICAgICAgICAgICAgICAgICAgICAgICAgLC4g4oCTICAtIC4sICDCsCAgICAgICAKICAgLMK3wrQgICAgJ2AgwrcuJyAgICAgICAgICAgICAvICAgJztcICcgICAgICAgICAgICAgICAgICAgICAgICc7XywuLiwgXyAgICAgJ2AuICcgICAKICAgIFxgOyBgwrc7wrcuICAgYMK3LCAgICAgICAgJzsgICAgOzonXCAgICAgICzCtyfCtCc7ICAgICAgICAgICAgIFw6Ojo6Ojo6Ojo6Oic7ICAgLCdcICAKICAgICA7ICAgLCdcOjpgwrcsICAgXCcgICAgICAgJzsgICA7Ojo7ICAgICAsJyAgLCcnXCAgICAgICAgICAgICAnXF87Ojo7OizCt8K0ICAuwrfCtDo6XOKAmAogICAgOyAgICwnOjonXDo6Oic7ICAgJzsgICAgICAnOyAgICc7Ojo7ICAgLCcgICwnOjo6J1wnICAgICAgICAgICAgICAgLCAnwrQgLsK3wrQ6Ojo6OjsnICAKICAgIDsgICA7Ojo6OyfCtzouJyAgLMK3J1wnICAgICAgJzsgICA7OjsgICwnICAsJzo6Ojo6OycgICAgICAgICAgICAgLsK0ICAuJzo6Ojo6OjvCt8K0JyAgIAogICAnOyAgJzs6IC3CtyAnwrQuIMK3J8K0Ojo6J1wnICAgICAgOyAgIDs6OyfCtCAsJzo6Ojo6OjsnICAnICAgICAgICAuwrfCtCAswrfCtDo6Ojo6O8K3wrQgICAgICAgCiAgIDsgICwtwrc6J8K0Olw6Ojo6Ojo6O8K3JyAgICAgICAnOyAgICfCtCAswrcnOjo6Ojo7JyAgICAgICAgICAswrfCtCAgLsK0Ozo64oCTwrd+XionwrQnO1zigJogICAKICAsJyAgJzs6Ojo6OjonXDs6wrcnwrQgICAgICAgICAgICAsJyAgICwuJ1w6OjvCt8K0ICAgICAgICAgICAgJy4sXyAsLiAtwrd+OionwrTCqMKvOlw6XCDCsAogIFzCty4swrdcOy3CtyAnwrQgICcgICAgICAgICAgICAgICBcYCrCtFw6OjpcOyAgICAg4oCYICAgICAgICAgIFw6Ojo6Ojo6Ojo6Ojo6Ojo6Ojo6XDsgIAogICBcOjpcOlwgICAgICAgICAgICAgICAgICAgICAgICdcOjo6XDsnICAgICAgICAgICAgICAgICAgIFw6O187Ojotwrd+XionwrTCqMKvJyAgICAKICAgIGAnwrc7wrcnICAgICAgICAgICAgICAgICAgICAgICAgIGAqwrTigJggICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAgICAg'
        print(base64.b64decode(data).decode())

    @staticmethod
    def request_json(url: str) -> dict:
        resp = request.urlopen(url)
        return json.loads(resp.read().decode(resp.headers.get_content_charset(), errors='replace'))

    @staticmethod
    def request_bin(url: str) -> bytes:
        resp = request.urlopen(url)
        return resp.read()

if __name__ == '__main__':
    sys.exit(App(sys.argv).exit_code)

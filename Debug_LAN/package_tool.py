"""Build the existing Debug_LAN source into a portable Windows tool; no hardware access."""
import argparse
import hashlib
import importlib.metadata
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import zipfile

SOURCE = Path(__file__).resolve().parent
ROOT = SOURCE.parent


def save(path, value):
    path.write_text(json.dumps(value, ensure_ascii=False, indent=2) + '\n', encoding='utf-8')


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    if os.name != 'nt' or sys.version_info[:3] != (3, 12, 10):
        raise RuntimeError('Build requires Windows x64 and Python 3.12.10.')
    for line in (SOURCE / 'requirements-build.lock').read_text().splitlines():
        if not line or line.startswith('#'):
            continue
        name, version = line.split('==')
        if importlib.metadata.version(name) != version:
            raise RuntimeError(f'Build dependency mismatch: {name}')
    if subprocess.check_output(['git', 'status', '--porcelain'], cwd=ROOT):
        raise RuntimeError('Commit source and documentation before packaging.')
    commit = subprocess.check_output(['git', 'rev-parse', 'HEAD'], cwd=ROOT, text=True).strip()
    version = (SOURCE / 'VERSION').read_text().strip()
    output = args.output.resolve()
    output.mkdir(parents=True, exist_ok=True)
    package = output / f'DebugLAN-{version}-windows-x64.zip'
    if package.exists():
        raise RuntimeError('Refusing to overwrite an existing candidate.')
    subprocess.run([sys.executable, '-B', '-m', 'unittest', 'discover', '-s', str(SOURCE / 'tests'), '-v'], cwd=ROOT, check=True)
    with tempfile.TemporaryDirectory(prefix='debug-lan-', dir=output) as directory:
        temp = Path(directory)
        subprocess.run([sys.executable, '-m', 'PyInstaller', '--noconfirm', '--clean', '--onedir', '--console', '--noupx',
                        '--name', 'debug-lan', '--distpath', str(temp / 'dist'), '--workpath', str(temp / 'build'),
                        '--specpath', str(temp), '--add-data', str(SOURCE / 'VERSION') + ';.', str(SOURCE / 'jlink_flash.py')], check=True)
        built = temp / 'dist/debug-lan'
        manifest = dict(manifestSchema=1, toolId='debug-lan', version=version, platform='windows', architecture='x64',
                        cliApiMajor=1, configSchemaRange=dict(min=1, max=1), entrypoint='debug-lan.exe',
                        upstreamCommit=commit, requirements=dict(jlinkSoftware='external'))
        save(built / 'tool-manifest.json', manifest)
        shutil.copy2(SOURCE / 'README.md', built / 'README.md')
        licenses = built / 'licenses'; licenses.mkdir()
        shutil.copy2(Path(sys.base_prefix) / 'LICENSE.txt', licenses / 'Python-LICENSE.txt')
        copied = 0
        distribution = importlib.metadata.distribution('pyinstaller')
        for item in distribution.files:
            if '.dist-info/licenses/' in item.as_posix():
                file = distribution.locate_file(item)
                if file.is_file():
                    shutil.copy2(file, licenses / file.name); copied += 1
        if copied == 0:
            raise RuntimeError('PyInstaller license and bootloader exception were not found.')
        with zipfile.ZipFile(package, 'w', zipfile.ZIP_DEFLATED, compresslevel=6) as archive:
            for file in sorted(built.rglob('*')):
                if file.is_file():
                    archive.write(file, file.relative_to(built).as_posix())
        extracted = temp / '中文 空格 工具'
        with zipfile.ZipFile(package) as archive:
            archive.extractall(extracted)
        environment = os.environ.copy()
        environment['PATH'] = str(Path(os.environ['SystemRoot']) / 'System32')
        environment.pop('PYTHONPATH', None); environment.pop('PYTHONHOME', None)
        executable = extracted / 'debug-lan.exe'
        result = subprocess.run([str(executable), '--version'], capture_output=True, text=True, check=True, env=environment, timeout=20)
        if result.stdout.strip() != version:
            raise RuntimeError('Executable version mismatch.')
        config = temp / 'remote-jlink.json'
        save(config, dict(ServerName='test.example.invalid', Port=19011, Device='STM32G474VE', Interface='SWD', SpeedKHz=8000))
        result = subprocess.run([str(executable), '--check', '--config', str(config)], capture_output=True, text=True, check=True, env=environment, timeout=20)
        if json.loads(result.stdout)['status'] != 'valid':
            raise RuntimeError('Offline executable check failed.')
        config.write_text('// comment\n' + config.read_text(encoding='utf-8').rstrip()[:-1] + ',\n}', encoding='utf-8')
        subprocess.run([str(executable), '--check', '--config', str(config)], capture_output=True, text=True, check=True, env=environment, timeout=20)
        digest = hashlib.sha256(package.read_bytes()).hexdigest()
        save(output / 'release-manifest.json', manifest | dict(packageName=package.name, packageBytes=package.stat().st_size, packageSha256=digest))
        (output / 'SHA256SUMS').write_text(f'{digest}  {package.name}\n', encoding='utf-8')
        save(output / 'verification-evidence.json', dict(version=version, commit=commit, packageSha256=digest, method='automated-offline',
             exitCode=0, checks=['source-unit-tests', 'final-zip-version', 'final-zip-offline-check-chinese-space-path', 'final-zip-jsonc-check'],
             hardware='not-tested', cleanWindows='not-tested', python='3.12.10', pyinstaller='6.22.3'))
        (output / 'release-notes.md').write_text(f'Debug_LAN {version} 开发内测。\n\nWindows x64目录式便携工具，完整解压后使用debug-lan.exe。'
            '包含Python运行时，用户无需安装Python；SEGGER工具需外部安装。源脚本与工具包共用同一实现。\n\n'
            '`--version`查看工具版本；`--check --config <remote-jlink.json>`仅离线校验，不解析DNS、不连接设备；本版新增JSONC注释/尾逗号兼容并拒绝重复键。'
            '烧录/GDB参数沿用源码入口，真实设备操作由用户明确执行。\n\n'
            '本包源码单元测试和最终ZIP解压后版本/离线配置检查通过。硬件未验收，纯净Windows未验收；未实际烧录或连接探针。\n', encoding='utf-8')
        print(json.dumps(dict(version=version, commit=commit, package=str(package), sha256=digest)))


if __name__ == '__main__':
    main()

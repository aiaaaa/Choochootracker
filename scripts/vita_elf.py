"""Check relocation records required when VitaSDK converts the linked ELF."""
import re

def validate_stdio(nm_report, link_map):
    if (not re.search(r'^\s*[0-9a-fA-F]+\s+T\s+__ssvfscanf_r\s*$',nm_report,re.MULTILINE)
        or not re.search(r'^LOAD /deps/newlib/c99-scanf\.o\s*$',link_map,re.MULTILINE)):
        raise RuntimeError('Vita ELF lacks the C99 string scanner; projects require hh formats.')
    return 'newlib-4.1.0-c99-string-scanner'

def validate_threads(nm_report, personal=False):
    defined = set(re.findall(r'^\s*[0-9a-fA-F]+\s+[Tt]\s+(pthread_\w+)\s*$',
                             nm_report, re.MULTILINE))
    # Ordinary may garbage-collect unused create/once: SDL uses native threads.
    required = ('pthread_cancel', 'pthread_create', 'pthread_once') if personal else ('pthread_cancel',)
    if not set(required).issubset(defined):
        raise RuntimeError('Vita ELF lacks active C++ pthread support. Compile/link '
                           'with -pthread; -lpthread alone leaves the weak '
                           'pthread_cancel activation proxy unresolved.')
    return list(required)

def validate_relocations(readelf_report):
    counts = {name: int(count) for name, count in re.findall(
        r"Relocation section '(\.rel\.[^']+)' at offset \S+ contains (\d+) entr(?:y|ies):",
        readelf_report)}
    # This C++ application has both absolute code references and global
    # constructors. A loadable SELF header alone does not prove relocatability.
    required = ('.rel.text', '.rel.init_array')
    if any(counts.get(name, 0) == 0 for name in required):
        raise RuntimeError('Vita ELF is missing code/constructor relocations. '
                           'Link with -Wl,-q,-z,nocopyreloc before vita-elf-create; '
                           'otherwise it may install but crash before main().')
    return {name: counts[name] for name in required}

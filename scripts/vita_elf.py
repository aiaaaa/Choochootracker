"""Check relocation records required when VitaSDK converts the linked ELF."""
import re

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

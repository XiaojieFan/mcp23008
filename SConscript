from building import *
Import('rtconfig')

src   = []
cwd   = GetCurrentDir()

# add mcp23008 src files.
if GetDepend('PKG_USING_MCP23008'):
    src += Glob('mcp23008.c')

if GetDepend('PKG_USING_MCP23008_SAMPLE'):
    src += Glob('mcp23008_sample.c')

# add mcp23008 include path.
path  = [cwd]

# add src and include to group.
group = DefineGroup('mcp23008', src, depend = ['PKG_USING_MCP23008'], CPPPATH = path)

Return('group')

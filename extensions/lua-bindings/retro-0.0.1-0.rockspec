-- «.build»	(to "build")
-- «.test»	(to "test")
-- http://luarocks.org/en/Creating_a_rock
-- http://luarocks.org/en/Rockspec_format


package="retro"
version="0.0.1-0"
source = {
  url = "http://angg.twu.net/RETRO/luaretro.tgz",
}
description = {
  summary = "A way to call Retro from Lua.",
  detailed = [[
    This was adapted from an example in the LuaRocks tutorial.
    Here we would put a detailed, typically
    paragraph-long description.
  ]],
  homepage = "http://angg.twu.net/RETRO/",
  license = "GPL3"         -- or whatever you like
}
dependencies = {
  "lua >= 5.1"
  -- If you depend on other rocks, add them here
}
build = {
  type = "builtin",
  modules = {
    retro = { "libretro.c", "luaretro.c" },
  },
}




--[[
# «build»  (to ".build")
# «test»  (to ".test")
--]]


-- Local Variables:
-- coding:             raw-text-unix
-- ee-anchor-format:   "«%s»"
-- End:

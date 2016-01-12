
# config.py


def can_build(platform):
	return platform=="x11" or platform=="windows"

def configure(env):
	env.Append(CPPPATH=["#modules/godotsteam/sdk/public/"])
	if env["platform"]== "x11":
		env.Append(LIBS=["steam_api"])
		#env.Append(RPATH=["."])
		if env["bits"]=="32":
			env.Append(RPATH=env.Literal('\\$$ORIGIN/linux32'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux32"])
		elif env["bits"]=="64":
			env.Append(RPATH=env.Literal('\\$$ORIGIN/linux64'))
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/linux64"])
	elif env["platform"] == "windows":
		if env["bits"]=="32":
			#env.Append(LIBS=["steam_api"])
			env.Append(LINKFLAGS=["steam_api.lib"])
			env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])
		elif env["bits"]=="64":
			# use 64bit steam_api?
			if (True): # Change to False if you have problems with "unresolved external symbols"
				env.Append(LINKFLAGS=["steam_api64.lib"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin/win64"])
			else:
				env.Append(LINKFLAGS=["steam_api.lib"])
				env.Append(LIBPATH=["#modules/godotsteam/sdk/redistributable_bin"])



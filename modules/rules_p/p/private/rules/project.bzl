load("//p/private:providers.bzl", "PInfo", "PProjectCompileInfo", "PProjectRuntimeInfo")

def PProject(ctx):
    toolchain = ctx.toolchains["//p:toolchain_type"]
    p = toolchain.p_info.p
    p_executable = p[DefaultInfo].files_to_run.executable
    dotnet_executable = toolchain.p_info.dotnet
    dotnet_path = dotnet_executable.files.to_list()[0]
    dotnet_runfiles = toolchain.p_info.dotnet_files

    out_dir = ctx.actions.declare_directory(ctx.label.name)
    dll = out_dir.short_path + "/PChecker/net8.0/" + ctx.label.name.replace("-", "_") + ".dll"

    sources = list(ctx.files.srcs)

    for dep in ctx.attr.deps:
        sources += dep[PProjectCompileInfo].input_files

    args = ["compile", "--projname", ctx.label.name.replace("-", "_"), "--outdir", out_dir.path]
    for src in sources:
        args += ["--pfiles", src.short_path]

    inputs = sources + p[DefaultInfo].default_runfiles.files.to_list() + p[DefaultInfo].data_runfiles.files.to_list() + dotnet_runfiles
    script = "export PATH=$PATH:$(pwd)/{} && DOTNET_CLI_HOME=$TMPDIR {} {}".format(dotnet_path.dirname, p_executable.path, " ".join(args))

    ctx.actions.run_shell(
        inputs = inputs,
        outputs = [out_dir],
        command = script,
    )

    compile_info = PProjectCompileInfo(
        name = ctx.label.name,
        input_files = ctx.files.srcs,
    )

    runtime_info = PProjectRuntimeInfo(
        name = ctx.label.name,
        dll = dll,
        directory = out_dir,
    )

    return [
        DefaultInfo(files = depset([out_dir])),
        compile_info,
        runtime_info,
    ]

p_project = rule(
    implementation = PProject,
    attrs = {
        "srcs": attr.label_list(
            doc = "",
            allow_files = True,
        ),
        "deps": attr.label_list(
            doc = "",
            providers = [PProjectCompileInfo],
        ),
    },
    toolchains = ["//p:toolchain_type"],
)

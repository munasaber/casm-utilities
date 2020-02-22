git_root()
{
    git rev-parse --show-toplevel
}

find_source_files()
{
    cd $(git_root)
    for d in ./include ./src ./lib ./lib-py; do
        find $d -type f -name "*.hpp" -o -name "*.cpp" -o -name "*.cxx"
    done
}

for f in $(find_source_files); do
    echo "Formatting $f ..."
    clang-format-10 -i $f
done

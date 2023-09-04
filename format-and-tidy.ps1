foreach ($folder in Get-ChildItem -Path Editor, Engine, Projects -Directory -Recurse)
{
    foreach ($file in Get-ChildItem -Path $folder.FullName -Include *.hh, *.cc -File)
    {
        clang-format -i $file.FullName
        # clang-tidy $file.FullName
    }
}

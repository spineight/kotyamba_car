##### interesting ideas that can be borrowed from google benchmark
1) https://github.com/google/benchmark/blob/v1.5.2/.github/workflows/build-and-test.yml
2) no release notes in repo - they are on github

3) https://github.com/google/benchmark/tree/v1.5.2/.github/ISSUE_TEMPLATE

4) How to release

https://github.com/google/benchmark/blob/v1.5.2/docs/releasing.md

How to release

    Make sure you're on master and synced to HEAD
    Ensure the project builds and tests run (sanity check only, obviously)
        parallel -j0 exec ::: test/*_test can help ensure everything at least passes
    Prepare release notes
        git log $(git describe --abbrev=0 --tags)..HEAD gives you the list of commits between the last annotated tag and HEAD
        Pick the most interesting.
    Create a release through github's interface
        Note this will create a lightweight tag.
        Update this to an annotated tag:
            git pull --tags
            git tag -a -f <tag> <tag>
            git push --force origin
            git push --tags

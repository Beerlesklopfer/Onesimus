#!/usr/bin/env bash
# Compare a release tag with the project version in CMakeLists.txt.
#
#   scripts/ci/check-tag-version.sh v1.0.0        -> ok, prints "version=1.0.0"
#   scripts/ci/check-tag-version.sh v1.0.0-rc.1   -> ok when project() says 1.0.0,
#                                                    prints "prerelease=true"
#
# The release workflow runs it first, so a tag pushed without bumping
# project(onesimus VERSION ...) fails before anything is built. With
# GITHUB_OUTPUT set the key=value lines go there as step outputs as well.
set -euo pipefail

tag="${1:-${GITHUB_REF_NAME:-}}"
[ -n "$tag" ] || { echo "usage: $0 <tag>  (e.g. v1.0.0)" >&2; exit 2; }

cd "$(dirname "$0")/../.."
# CMAKELISTS: another file to read (the script's own tests use it).
cmakelists="${CMAKELISTS:-CMakeLists.txt}"

# Tag: v<MAJOR>.<MINOR>.<PATCH>, optionally -<pre-release> (v1.0.0-rc.1).
if [[ ! "$tag" =~ ^v([0-9]+\.[0-9]+\.[0-9]+)(-([0-9A-Za-z.-]+))?$ ]]; then
    echo "::error::Tag '$tag' is not of the form vMAJOR.MINOR.PATCH[-PRERELEASE] (e.g. v1.0.0)" >&2
    exit 1
fi
tag_version="${BASH_REMATCH[1]}"
pre="${BASH_REMATCH[3]:-}"

# project(onesimus VERSION X.Y.Z ...) - the one place the triplet is set.
project_version="$(sed -n 's/^project([[:space:]]*onesimus[[:space:]]\{1,\}VERSION[[:space:]]\{1,\}\([0-9][0-9.]*\).*/\1/p' "$cmakelists" | head -1)"
[ -n "$project_version" ] || { echo "::error::No project(onesimus VERSION ...) in CMakeLists.txt" >&2; exit 1; }

if [ "$tag_version" != "$project_version" ]; then
    echo "::error::Tag $tag does not match the project version $project_version in CMakeLists.txt." >&2
    echo "Bump project(onesimus VERSION $tag_version) in CMakeLists.txt, commit, and tag that commit." >&2
    exit 1
fi

prerelease=false
[ -n "$pre" ] && prerelease=true
echo "Tag $tag matches project version $project_version (pre-release: $prerelease)"
out="version=$project_version
prerelease=$prerelease"
echo "$out"
if [ -n "${GITHUB_OUTPUT:-}" ]; then
    echo "$out" >> "$GITHUB_OUTPUT"
fi

#!/bin/sh
#***************************************************************************
#                                  _   _ ____  _
#  Project                     ___| | | |  _ \| |
#                             / __| | | | |_) | |
#                            | (__| |_| |  _ <| |___
#                             \___|\___/|_| \_\_____|
#
# Copyright (C) Daniel Stenberg, <daniel@haxx.se>, et al.
#
# This software is licensed as described in the file COPYING, which
# you should have received as part of this distribution. The terms
# are also available at https://curl.se/docs/copyright.html.
#
# You may opt to use, copy, modify, merge, publish, distribute and/or sell
# copies of the Software, and permit persons to whom the Software is
# furnished to do so, under the terms of the COPYING file.
#
# This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
# KIND, either express or implied.
#
# SPDX-License-Identifier: curl
#
###########################################################################

set -eu

# this should ideally be passed in
timestamp=${1:-unknown}
version=${2:-unknown}
tag=$(echo "curl-$version" | tr '.' '_')

cat <<MOO
# Release tools used for curl $version

The following tools and their Debian package version numbers were used to
produce this release tarball.

MOO

exists=$(command -v dpkg 2>/dev/null)
if test ! -e "$exists"; then
  echo "(unknown, could not find dpkg)"
  exit
fi

debian() {
  echo "- $1: $(dpkg -l "$1" | grep ^ii | awk '{print $3}')"
}
debian autoconf
debian automake
debian libtool
debian make
debian perl
debian git

cat <<MOO

# Reproduce the tarball

- Clone the repo and checkout the tag: $tag
- Install the same set of tools + versions as listed above

## Do a standard build

- autoreconf -fi
- ./configure [...]
- make

## Generate the tarball with the same timestamp

- export SOURCE_DATE_EPOCH=$timestamp
- ./scripts/maketgz [version]

MOO

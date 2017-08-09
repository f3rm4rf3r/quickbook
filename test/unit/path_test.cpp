/*=============================================================================
    Copyright (c) 2015 Daniel James

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "path.hpp"
#include <boost/detail/lightweight_test.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/predef.h>

void file_path_to_url_tests() {
    using boost::filesystem::path;
    using quickbook::file_path_to_url;

    BOOST_TEST_EQ("a/b", file_path_to_url(path("a/b")));
    BOOST_TEST_EQ("../a/b", file_path_to_url(path("../a/b")));
    BOOST_TEST_EQ("A%20B%2bC%2520", file_path_to_url(path("A B+C%20")));
    BOOST_TEST_EQ("file:///a/b", file_path_to_url(path("/a/b")));

#if BOOST_OS_WINDOWS
    BOOST_TEST_EQ("file:///a", file_path_to_url(path("\\a")));
    BOOST_TEST_EQ("file:///c:/", file_path_to_url(path("c:\\")));
    BOOST_TEST_EQ("file:///c:/foo/bar", file_path_to_url(path("c:\\foo\\bar")));
#endif
}

void normalize_path_tests() {
    using boost::filesystem::current_path;
    using boost::filesystem::canonical;
    using boost::filesystem::path;
    using quickbook::normalize_path;

    // TODO: While these paths are equal, they don't have the same string
    // representation on windows, because fs::canonical creates a path like
    // c:/path\a\c instead of c:\path\a\c
    // This is because the iterator it uses, uses 'separator' rather than
    // 'preferred_separator'.
    BOOST_TEST(current_path() == normalize_path(path("")));
    BOOST_TEST(current_path() == normalize_path(path(".")));
    BOOST_TEST(current_path().parent_path() == normalize_path(path("..")));
    BOOST_TEST(current_path()/"a"/"c" == normalize_path(path("a///b/..///c")));
    BOOST_TEST(current_path().parent_path()/"a"/"c" == normalize_path(path("..//a//b/..///c")));
    BOOST_TEST(current_path().parent_path()/"c" == normalize_path(path("a//b/../..//..//c")));
}

int main() {
    file_path_to_url_tests();
    normalize_path_tests();
    return boost::report_errors();
}

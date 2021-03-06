// Jubatus: Online machine learning framework for distributed environment
// Copyright (C) 2011 Preferred Infrastructure and Nippon Telegraph and Telephone Corporation.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License version 2.1 as published by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

#include <gtest/gtest.h>

#include "stat_client.hpp"
#include "test_util.hpp"

static const int PORT = 65435;

namespace {

class stat_test : public ::testing::Test {
 protected:
  pid_t child_;

  stat_test() {
    child_ = fork_process("stat", PORT, "./test_input/config.stat.json");
  }

  virtual ~stat_test() {
    kill_process(child_);
  }

  virtual void restart_process() {
    kill_process(this->child_);
    this->child_ = fork_process("stat", PORT, "./test_input/config.stat.json");
  }
};

TEST_F(stat_test, small) {
  jubatus::client::stat c("localhost", PORT, 10);
  c.push("", "hoge", 12);
  ASSERT_DOUBLE_EQ(12.0, c.sum("", "hoge"));

  // TODO(kuenishi): add more tests
  ASSERT_DOUBLE_EQ(.0, c.stddev("", "hoge"));
  ASSERT_DOUBLE_EQ(12.0, c.max("", "hoge"));
  ASSERT_DOUBLE_EQ(12.0, c.min("", "hoge"));

  ASSERT_DOUBLE_EQ(0., c.entropy("", "hoge"));

  ASSERT_EQ(true, c.save("", __func__));
  ASSERT_EQ(true, c.clear(""));
  ASSERT_EQ(true, c.load("", __func__));
}

TEST_F(stat_test, api_get_client) {
  jubatus::client::stat cli("localhost", PORT, 10);
  cli.clear("");

  msgpack::rpc::client& conn = cli.get_client();
  EXPECT_NO_THROW(conn.close());
}

}  // namespace

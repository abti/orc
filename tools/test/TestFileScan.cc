/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "orc/ColumnPrinter.hh"
#include "orc/OrcFile.hh"

#include "Adaptor.hh"
#include "ToolTest.hh"

#include "wrap/gmock.h"
#include "wrap/gtest-wrapper.h"

TEST (TestFileScan, testNominal) {
  const std::string pgm = findProgram("tools/src/orc-scan");
  const std::string file = findExample("TestOrcFile.testSeek.orc");
  std::string output;
  std::string error;
  EXPECT_EQ(0, runProgram({pgm, file}, output, error));
  EXPECT_EQ("Rows: 32768\nBatches: 33\n", output);
  EXPECT_EQ("", error);

  EXPECT_EQ(0, runProgram({pgm, "-b", "256", file}, output, error));
  EXPECT_EQ("Rows: 32768\nBatches: 131\n", output);
  EXPECT_EQ("", error);

  EXPECT_EQ(0, runProgram({pgm, "-b256", file}, output, error));
  EXPECT_EQ("Rows: 32768\nBatches: 131\n", output);
  EXPECT_EQ("", error);

  EXPECT_EQ(0, runProgram({pgm, "--batch", "256", file}, output, error));
  EXPECT_EQ("Rows: 32768\nBatches: 131\n", output);
  EXPECT_EQ("", error);

  EXPECT_EQ(0, runProgram({pgm, "--batch=256", file}, output, error));
  EXPECT_EQ("Rows: 32768\nBatches: 131\n", output);
  EXPECT_EQ("", error);
}

TEST (TestFileScan, testBadCommand) {
  const std::string pgm = findProgram("tools/src/orc-scan");
  const std::string file = findExample("TestOrcFile.testSeek.orc");
  std::string output;
  std::string error;
  EXPECT_EQ(1, runProgram({pgm, file, "-b"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("orc-scan: option requires an argument -- b\n"
            "Usage: orc-scan [-h] [--help]\n"
            "                [-b<size>] [--batch=<size>] <filename>\n", error);

  EXPECT_EQ(1, runProgram({pgm, file, "-b", "20x"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("The --batch parameter requires an integer option.\n", error);

  EXPECT_EQ(1, runProgram({pgm, file, "-b", "x30"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("The --batch parameter requires an integer option.\n", error);

  EXPECT_EQ(1, runProgram({pgm, file, "--batch"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("orc-scan: option `--batch' requires an argument\n"
            "Usage: orc-scan [-h] [--help]\n"
            "                [-b<size>] [--batch=<size>] <filename>\n", error);

  EXPECT_EQ(1, runProgram({pgm, file, "--batch", "20x"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("The --batch parameter requires an integer option.\n", error);

  EXPECT_EQ(1, runProgram({pgm, file, "--batch", "x30"}, output, error));
  EXPECT_EQ("", output);
  EXPECT_EQ("The --batch parameter requires an integer option.\n", error);
}

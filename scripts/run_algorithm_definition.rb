# アルゴリズムの使用方法を定義をして順次コンパイル・実行をし、ログを出力する生成するスクリプト
#
# 変数sizes, algorithms, te に実行したい順に指定する
# 詳しくはソースコードを読むこと
#
# C のコードの書かれた algorithm_definition ファイルが生成される。
# C ではこのファイルをインクルードしている。


#############
# 設定
# 使いたいものはコメントをはずし、使わないものはコメントをつける
# また、複数選択すると、全ての組み合わせパターンを順番に実行する
#

# 用いる盤面サイズを選択
# 複数可
sizes = [
  [4,4],
#   [4,5],
  [5,4],
  [5,5],
#   [5,6],
]

# 用いるアルゴリズムを選択
# 複数可だが、そのときは、組み合わせを行うことになる
# name に日本語などのマルチバイト文字や半角スペースは書いてはいけない
algorithms = [
#   {
#     "name" => "Random",
#     "code" => "getBitByRandom(mobility)"
#   },
#   {
#     "name" => "PriorityTables",
#     "code" => "getBitByTable(mobility)"
#   },
#   {
#     "name" => "OpenTheory",
#     "code" => "getBitByOpenTheory(mobility,o,1)"
#   },
#   {
#     "name" => "ExpectationMethod",
#     "code" => "getBitByExpectationMethod(mobility,o,1)"
#   },
#   {
#     "name" => "NegaalphaMethod",
#     "code" => "getBitByNegaalpha(mobility,o,1024)"
#   },
  {
    "name" => "MNM",
    "code" => "getBitByMNM(mobility,o,1)"
  },
]

# アルゴリズムの有効範囲を手数で指定（パターンを並べる）(序盤：1手目～X手目、終盤：最終手-Y手目～最終手)
# 序盤・中盤・終盤で枝刈りの性質が違うため、その性質が変わる場所を探す
# 複数可
te = [
  [3, 3],
  [3, 4],
  [3, 5],

  [4, 3],
  [4, 4],
  [4, 5],

  [5, 3],
  [5, 4],
  [5, 5],
]

# 結果の表示の仕方
# VIEW=TRUEのほうを使うと、棋譜をボードを使って結果表示する。
# もうひとつにすると、棋譜を表示せず、演算速度等を出力する。

# view_kifu_board = "VIEW=TRUE"
view_kifu_board = ""






#############
# コード


# コマンドを実行して、出力する関数
def run_command(cmd)
  puts cmd
#   IO.popen(cmd, "r+").each_line {|line|
#     puts line
#   }
  system(cmd)
end


# makeとその実行をする関数
def make_and_run(algorithm1, algorithm2, algorithm3, size_x, size_y, depth1, depth2, time, view_kifu_board)
  # 情報の表示(ログファイルには出力されない)
  puts <<-EOS

________________________________________________________________________________________

    Time(Batch Started)     = #{time.strftime("%Y/%m/%d %H:%M:%S")}
    Time(Now)               = #{Time.now.strftime("%Y/%m/%d %H:%M:%S")}
    Algorithm Combination   = #{algorithm1['name']} / #{algorithm2['name']} / #{algorithm3['name']}
    Board Size              = #{size_x}x#{size_y}
    Midgame                 = #{depth1}..#{size_x*size_y - 4 - depth2}

  EOS

  target = "algorithm_definition"
  max_depth = (size_x*size_y-4)

  file = open("src/algorithm_definition", "w")
  file.puts "// #{algorithm1['name']} #{algorithm2['name']} #{algorithm3['name']}"

  if (algorithm1["name"] == algorithm2["name"]) and (algorithm2["name"] == algorithm3["name"]) then
    file.puts <<-EOS
        Bitboard m = #{algorithm1["code"]};
    EOS
  else
    file.puts <<-EOS
      Bitboard m;
      int mobcount = count(o->black | o->white) - 4 + 1; // [TODO] ちょっと遅くなるので仮使用
      if ( mobcount <= #{depth1} )
        m = #{algorithm1["code"]};
      else if ( mobcount < #{size_x*size_y - 4 - depth2} )
        m = #{algorithm2["code"]};
      else
        m = #{algorithm3["code"]};
    EOS
  end

  file.close

  filename = "logs/#{time.strftime("%Y-%m-%d-%H-%M-%S")}.csv"
  run_command("make #{target} SIZE_X=#{size_x} SIZE_Y=#{size_y} DEPTH_START=#{depth1} DEPTH_END=#{size_x*size_y - 4 - depth2} ANALYZE=PERFECT #{view_kifu_board} ALGORITHM_NAME=#{algorithm1['name']}/#{algorithm2['name']}/#{algorithm3['name']}")
#  run_command("echo algorithm is #{algorithm['name']} #{depth_start}-#{depth_end}>> logs/#{time}.log")
  run_command("bin/bb#{target} >> #{filename}")

  run_command("tail #{filename} -n 1")

end

# 設定した順で make と実行を行う
time = Time.now

run_command("echo \"algorithm name,size x,size y,depth start,depth end,nodes,time(ms),time per nodes\" >> logs/#{time.strftime("%Y-%m-%d-%H-%M-%S")}.csv")


############################################################################################
# (10).times { |x|
#   make_and_run(algorithms[0], algorithms[0], 5,5,1,21, time, view_kifu_board)
# }

#   make_and_run(algorithms[0], algorithms[0], algorithms[0], 4,4,4,9, time, view_kifu_board)
#   make_and_run(algorithms[0], algorithms[0], algorithms[0], 4,5,4,13, time, view_kifu_board)
#   make_and_run(algorithms[0], algorithms[0], algorithms[0], 5,5,4,18, time, view_kifu_board)
#   make_and_run(algorithms[0], algorithms[0], algorithms[0], 5,6,4,23, time, view_kifu_board)

#   make_and_run(algorithms[0], algorithms[1], algorithms[0], 5,6,4,23, time, view_kifu_board)
#   make_and_run(algorithms[0], algorithms[1], algorithms[1], 5,6,4,23, time, view_kifu_board)
#   make_and_run(algorithms[1], algorithms[1], algorithms[0], 5,6,4,23, time, view_kifu_board)
#   make_and_run(algorithms[1], algorithms[1], algorithms[1], 5,6,4,23, time, view_kifu_board)
#   make_and_run(algorithms[0], algorithms[0], sizes[0][0],sizes[0][1],1,sizes[0][0]*sizes[0][1]-4, time, view_kifu_board)

# make_and_run(algorithms[0], algorithms[0], 5,6,1,26, time, view_kifu_board)

# sizes.each{|size|
#   make_and_run(algorithms[0], algorithms[0], size[0],size[1],1,size[0]*size[1]-4, time, view_kifu_board)
# }
############################################################################################

# 複数の手法の組み合わせを順に実行
sizes.each{|size|
  algorithms.length.times {|a1|
    algorithms.length.times {|a2|
      algorithms.length.times {|a3|
        te.each {|a|
          make_and_run(algorithms[a1], algorithms[a2], algorithms[a3], size[0], size[1], a[0], a[1], time, view_kifu_board)
        }
      }
    }
  }
}

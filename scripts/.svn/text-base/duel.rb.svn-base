#
# リバーシで対決
#
# 2つのリバーシで対決
#
# 目的：
#   ミニマックス戦略同士の対決は完全プレイの棋譜になるため、棋譜が正しいかどうかの検証に使う
#
#   あまり使用することはない
#
# 使い方：
#   src/setting.h の VIEW_KIFU と INIT_BOARD と VIEW_FIRST_MOB のコメントをはずして定義済みにし、make する。
#   これにより生成されたバイナリファイルをこのスクリプトのbinary変数に書き加える
#   ruby duel.rb を実行することで結果を見る
#
# [TODO] 自動的に対決させたいバイナリをmakeするようにする
#  方法がややこしいので
#  binary変数に指定したバイナリからmakeすべきものを自動的に求め、makeと対決の実行を行い、結果をlogsに出力する
#
# author: hirooka
#


# 初期盤面
# ret = "#{ARGV[0]} #{ARGV[1]} #{ARGV[2]}"
ret = "18049582881570816 9077567998918656 0"
# ret = "17924361838745616384 15859905474854912 0"
# ret = "4521810288705536 17935700311888887808 1"
# ret = "4538921438412800 8097560090942373888 0"
# ret = "8647122940539699200 9293231088673488896 0"
# puts ret;
binary = ["bin/bbnegamax","bin/bbnegaalpha"]

30.times {|x|
  # 表示用
#   IO.popen("#{binary[0]} #{ret}", "r+").each_line {|line|
#   IO.popen("#{binary[1]} #{ret}", "r+").each_line {|line|
#   IO.popen("#{binary[x%2]} #{ret}", "r+").each_line {|line|
  IO.popen("#{binary[(x+1)%2]} #{ret}", "r+").each_line {|line|
    puts line
  }
  # 受け渡し用
#   ret = IO.popen("#{binary[0]} #{ret}", "r+") {|io|
#   ret = IO.popen("#{binary[1]} #{ret}", "r+") {|io|
#   ret = IO.popen("#{binary[x%2]} #{ret}", "r+") {|io|
  ret = IO.popen("#{binary[(x+1)%2]} #{ret}", "r+") {|io|
    io.gets
  }
}



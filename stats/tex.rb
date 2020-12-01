require 'yaml'

def color(player)
  return 'violet' if player[:greedy]
  return 'Aquamarine' if player[:random]
  'YellowOrange'
end

def letter(player)
  return 'g' if player[:greedy]
  return 'r' if player[:random]
  'p'
end

def subfigure(players)
  bars = players.each_with_index.map do |player, i|
    wins_and_ties = player[:won] + player[:tied]
    <<~BAR
    \t\\bcbar[color=#{color(player)}, label=P#{i+1} (#{letter(player)}), value=#{wins_and_ties}]{#{wins_and_ties / 1000.0}}
    BAR
  end
  <<~SUB_FIG
  \\begin{subfigure}[a]{0.5\\textwidth}
    \\subcaption{}
    \\begin{bchart}[step=2,max=10,unit=k, scale=0.5]
      #{bars.join("\t\\smallskip\n")}
      \\bcxlabel{number of wins + ties}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
  SUB_FIG
end

def figure_of_subfigures(filenames)
  filenames.map {|fn| subfigure(YAML.load(File.read("#{fn}.yml")).values)}.join
end

puts figure_of_subfigures(ARGV)

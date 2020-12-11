require 'yaml'

players = YAML.load(File.read(ARGV[0]))

step = 1
max = 5

def bars_for_player(player_id, bird, players)
pid = player_id.to_s[-1].to_i + 1
times_taken = players[player_id][:card_in_games][bird][1]
won = players[player_id][:had_card_and_won][bird][1]
lost = players[player_id][:had_card_and_lost][bird][1]

<<-BARS
      \\bcbar[color=YellowOrange, label=P#{pid} took it, value=#{times_taken}]{#{times_taken/1000.0}}
      \\smallskip
      \\bcbar[color=violet, label=P#{pid} took it and won, value=#{won}]{#{won/1000.0}}
      \\smallskip
      \\bcbar[color=Aquamarine, label=P#{pid} took it and lost, value=#{lost}]{#{lost/1000.0}}
BARS
end

def multibars_for_player(player_id, bird, players)
times_taken = players[player_id][:card_in_games][bird][1] + players[player_id][:card_in_games][bird][2]
pid = player_id.to_s[-1].to_i + 1
won1 = players[player_id][:had_card_and_won][bird][1]
lost1 = players[player_id][:had_card_and_lost][bird][1]
won2 = players[player_id][:had_card_and_won][bird][2]
lost2 = players[player_id][:had_card_and_lost][bird][2]

<<-BARS
      \\bcbar[color=YellowOrange, label=P#{pid} took it, value=#{times_taken}]{#{times_taken/1000.0}}
      \\smallskip
      \\bcbar[color=violet, label=P#{pid} took 1 and won, value=#{won1}]{#{won1/1000.0}}
      \\smallskip
      \\bcbar[color=Aquamarine, label=P#{pid} took 1 and lost, value=#{lost1}]{#{lost1/1000.0}}
      \\smallskip
      \\bcbar[color=violet, label=P#{pid} took 2 and won, value=#{won2}]{#{won2/1000.0}}
      \\smallskip
      \\bcbar[color=Aquamarine, label=P#{pid} took 2 and lost, value=#{lost2}]{#{lost2/1000.0}}
BARS
end


tex = <<-TEX
\\begin{figure}
  \\captionsetup[subfigure]{justification=centering}
  \\begin{subfigure}[a]{0.5\\textwidth}
    \\subcaption{bird 1}
    \\begin{bchart}[step=#{step},max=#{max},unit=k,scale=0.5]
#{players.keys.map {|p| multibars_for_player(p, 'bird 01', players)}.join("      \\bigskip\n")}      \\bcxlabel{number of games}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
  \\begin{subfigure}[a]{0.5\\textwidth}
    \\subcaption{bird 2}
    \\begin{bchart}[step=#{step},max=#{max},unit=k,scale=0.5]
#{players.keys.map {|p| multibars_for_player(p, 'bird 02', players)}.join("      \\bigskip\n")}      \\bcxlabel{number of games}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
  \\begin{subfigure}[a]{0.5\\textwidth}
    \\subcaption{bird 3}
    \\begin{bchart}[step=#{step},max=#{max},unit=k,scale=0.5]
#{players.keys.map {|p| bars_for_player(p, 'bird 03', players)}.join("      \\bigskip\n")}      \\bcxlabel{number of games}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
  \\begin{subfigure}[a]{0.5\\textwidth}
    \\subcaption{bird 4}
    \\begin{bchart}[step=#{step},max=#{max},unit=k,scale=0.5]
#{players.keys.map {|p| bars_for_player(p, 'bird 04', players)}.join("      \\bigskip\n")}      \\bcxlabel{number of games}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
  \\begin{subfigure}[a]{\\textwidth}
    \\centering
    \\subcaption{bird 5}
    \\begin{bchart}[step=#{step},max=#{max},unit=k,scale=0.5]
#{players.keys.map {|p| bars_for_player(p, 'bird 05', players)}.join("      \\bigskip\n")}      \\bcxlabel{number of games}
    \\end{bchart}
    \\vspace*{0.5em}
  \\end{subfigure}
\\caption{Bird Lady with #{players.count} players, birds, 10k games}
\\label{fig:bird_lady_#{players.count}p_birds}
\\end{figure}
TEX

puts tex

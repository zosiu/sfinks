#!/usr/bin/env ruby

require 'json'
require 'yaml'

out_filename = ARGV[1]
in_filename = ARGV[0]
history = JSON.parse(File.read(in_filename))['history']

players = history.first['results'].each_with_object({}) do |obj, hsh|
  hsh[obj['key']] = { id: obj['key'],
                      greedy: obj['value']['greedy'],
                      random: obj['value']['exploration_rate'] == 1,
                      won: 0,
                      lost: 0,
                      tied: 0,
                      winning_scores: Hash.new(0),
                      losing_scores: Hash.new(0),
                      winning_actions: Hash.new(0),
                      losing_actions: Hash.new(0) }
end

history.each do |record|
  scores = record['results'].map { |res| res['value']['result'] }
  max_score = scores.max
  tie = scores.count(max_score) > 1
  players.each do |(pid, pdata)|
    pres = record['results'].find { |res| res['key'] == pid }
    if pres['value']['result'] == max_score
      pdata[:winning_scores][pres['value']['result']] +=1
      actions = record['actions'].select { |act| act['tuple_element0'] == pid }.map { |x| x['tuple_element1'] }
      pdata[:winning_actions][actions] += 1
      if tie
        pdata[:tied] += 1;
      else
        pdata[:won] += 1;
      end
    else
      pdata[:losing_scores][pres['value']['result']] += 1
      actions = record['actions'].select { |act| act['tuple_element0'] == pid }.map { |x| x['tuple_element1'] }
      pdata[:losing_actions][actions] += 1
      pdata[:lost] += 1;
    end
  end
end

players_reduced = players.map do |(k, v)|
  v[:winning_scores] = v[:winning_scores].sort_by { |(k, v)| -k }
  v[:losing_scores] = v[:losing_scores].sort_by { |(k, v)| k }

  most_common_winning_actions = v[:winning_actions].values.sort.last(5)
  v[:winning_actions] = v[:winning_actions].select { |_, v| most_common_winning_actions.include?(v) }.to_h
  most_common_losing_actions = v[:losing_actions].values.sort.last(5)
  v[:losing_actions] = v[:losing_actions].select { |_, v| most_common_losing_actions.include?(v) }.to_h

  [k, v]
end.to_h

File.open("#{out_filename}.yml", 'w') { |file| file.write(players_reduced.to_yaml) }

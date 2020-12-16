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
                      actions_when_won: Hash.new(0),
                      first_action_when_won: Hash.new(0),
                      actions_when_tied: Hash.new(0),
                      first_action_when_tied: Hash.new(0),
                      actions_when_lost: Hash.new(0),
                      first_action_when_lost: Hash.new(0),
                      winning_scores: Hash.new(0),
                      losing_scores: Hash.new(0) }
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
      if tie
        pdata[:tied] += 1;
        pdata[:first_action_when_tied][actions.first] += 1
        pdata[:actions_when_tied][actions] += 1
      else
        pdata[:won] += 1;
        pdata[:first_action_when_won][actions.first] += 1
        pdata[:actions_when_won][actions] += 1
      end
    else
      pdata[:losing_scores][pres['value']['result']] += 1
      actions = record['actions'].select { |act| act['tuple_element0'] == pid }.map { |x| x['tuple_element1'] }
      pdata[:lost] += 1;
      pdata[:actions_when_lost][actions] += 1
      pdata[:first_action_when_lost][actions.first] += 1
    end
  end
end

players_reduced = players.map do |(k, v)|
  v[:winning_scores] = v[:winning_scores].sort_by { |(k, v)| -k }
  v[:losing_scores] = v[:losing_scores].sort_by { |(k, v)| k }

  most_common_winning_actions = v[:actions_when_won].values.sort.last(5)
  v[:actions_when_won] = v[:actions_when_won].select { |_, v| most_common_winning_actions.include?(v) }.to_h
  most_common_winning_actions = v[:actions_when_tied].values.sort.last(5)
  v[:actions_when_tied] = v[:actions_when_tied].select { |_, v| most_common_winning_actions.include?(v) }.to_h
  most_common_losing_actions = v[:actions_when_lost].values.sort.last(5)
  v[:actions_when_lost] = v[:actions_when_lost].select { |_, v| most_common_losing_actions.include?(v) }.to_h

  [k, v]
end.to_h

File.open("#{out_filename}.yml", 'w') { |file| file.write(players_reduced.to_yaml) }

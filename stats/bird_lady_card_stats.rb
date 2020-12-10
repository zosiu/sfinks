#!/usr/bin/env ruby

require 'json'
require 'yaml'

out_filename = ARGV[1]
in_filename = ARGV[0]
history = JSON.parse(File.read(in_filename))['history']

players = history.first['results'].each_with_object(Hash.new(0)) do |obj, hsh|
  hsh[obj['key']] = { player_id: obj['key'],
                      had_card_and_won: Hash.new { |h,key| h[key] = Hash.new(0) },
                      had_card_and_lost: Hash.new { |h,key| h[key] = Hash.new(0) },
                      card_in_games: Hash.new { |h,key| h[key] = Hash.new(0) } }
end

history.each do |record|
  max_score = record['results'].map { |res| res['value']['result'] }.max
  players.each do |(pid, pdata)|
    won = record['results'].find { |res| res['key'] == pid }['value']['result'] == max_score
    actions = record['actions'].select { |act| act['tuple_element0'] == pid }.map { |x| x['tuple_element1'] }
    cards = actions.each_with_object(Hash.new(0)) do |action, hsh|
      hsh[action['card_1']] += 1
      hsh[action['card_2']] += 1
      hsh[action['mystery_bird']] += 1
    end
      cards.each { |card, count| pdata[:card_in_games][card][count] += 1 }
    if won
      cards.each { |card, count| pdata[:had_card_and_won][card][count] += 1 }
    else
      cards.each { |card, count| pdata[:had_card_and_lost][card][count] += 1 }
    end
  end
end

File.open("#{out_filename}.yml", 'w') { |file| file.write(players.to_yaml) }

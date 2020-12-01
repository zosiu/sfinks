#!/usr/bin/env ruby

require 'json'
require 'yaml'

out_filename = ARGV[1]
in_filename = ARGV[0]
history = JSON.parse(File.read(in_filename))['history']

players = history.first['results'].each_with_object(Hash.new(0)) do |obj, hsh|
  hsh[obj['key']] = { player_id: obj['key'],
                      won: 0,
                      lost: 0,
                      cards_taken_when_won: Hash.new(0),
                      cards_taken_when_lost: Hash.new(0),
                      mystery_birds_taken_when_won: Hash.new(0),
                      mystery_birds_taken_when_lost: Hash.new(0),
                      slices_taken_when_won: Hash.new(0),
                      slices_taken_when_lost: Hash.new(0) }
end

history.each do |record|
  max_score = record['results'].map { |res| res['value']['result'] }.max
  players.each do |(pid, pdata)|
    pres = record['results'].find { |res| res['key'] == pid }
    actions = record['actions'].select { |act| act['tuple_element0'] == pid }.map { |x| x['tuple_element1'] }
    if pres['value']['result'] == max_score
      pdata[:won] += 1
      actions.each do |action|
        pdata[:slices_taken_when_won][action['board_slice']] += 1
        pdata[:cards_taken_when_won][action['card_1']] += 1
        pdata[:cards_taken_when_won][action['card_2']] += 1
        pdata[:mystery_birds_taken_when_won][action['mystery_bird']] += 1
      end
    else
      pdata[:lost] += 1
      actions.each do |action|
        pdata[:slices_taken_when_lost][action['board_slice']] += 1
        pdata[:cards_taken_when_lost][action['card_1']] += 1
        pdata[:cards_taken_when_lost][action['card_2']] += 1
        pdata[:mystery_birds_taken_when_lost][action['mystery_bird']] += 1
      end
    end
  end
end

players.each do |(pid, pdata)|
  pdata[:slices_taken_when_won].each { |k, v| pdata[:slices_taken_when_won][k] /= pdata[:won].to_f }
  pdata[:cards_taken_when_won].each { |k, v| pdata[:cards_taken_when_won][k] /= pdata[:won].to_f }
  pdata[:mystery_birds_taken_when_won].each { |k, v| pdata[:mystery_birds_taken_when_won][k] /= pdata[:won].to_f }

  pdata[:slices_taken_when_lost].each { |k, v| pdata[:slices_taken_when_lost][k] /= pdata[:lost].to_f }
  pdata[:cards_taken_when_lost].each { |k, v| pdata[:cards_taken_when_lost][k] /= pdata[:lost].to_f }
  pdata[:mystery_birds_taken_when_lost].each { |k, v| pdata[:mystery_birds_taken_when_lost][k] /= pdata[:lost].to_f }
end

File.open("#{out_filename}.yml", 'w') { |file| file.write(players.to_yaml) }

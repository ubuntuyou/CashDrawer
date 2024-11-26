require 'glimmer-dsl-libui'
require 'time'
require 'money'

class CashDrawer

	include Glimmer
	
	Money.locale_backend = :currency
	Money.default_currency = 'USD'
	Money.rounding_mode = BigDecimal::ROUND_HALF_UP
	
	Drawer = Struct.new(
		:penny,
		:nickel,
		:dime,
		:quarter,
		:ones,
		:fives,
		:tens,
		:twentys,
		:fiftys,
		:bennys
	)
	
	Available = Struct.new(
		:checks,
		:bank,
		:mvdue,
		:petty,
		:cbag
	)
	
	Payable = Struct.new(
		:office,
		:copy,
		:motors,
		:guns,
		:prints
	)
	
	Total = Struct.new( :date, :dtotal, :onhand, :payable, :diff )
	
	attr_accessor :totals
	attr_accessor :ones, :fives, :tens, :twentys, :fiftys, :bennys
	attr_accessor :penny, :nickel, :dime, :quarter
	attr_accessor :checks, :petty, :bank, :mvdue, :cbag
	attr_accessor :office, :copy, :motors, :guns, :prints
	attr_accessor :date, :dtotal, :onhand, :payable, :diff
	

	def initialize
		@totals = [
			Total.new('2024-11-23', '$17.99', '$534.88', '$1899.37', '$1364.49' )
		]
	end
	
	
	def file_menu
		menu('File') {
			menu_item('Open') {
				on_clicked do
					file = open_file
					puts file unless file.nil?
				end
			}
			
			menu_item('Save') {
				on_clicked do
					file = save_file
					puts file unless file.nil?
					$stdout.flush
				end
			}
			
			quit_menu_item
		}
	end
	
	def help_menu
		menu('Help') {
			about_menu_item {
				on_clicked do
					msg_box('Cash Drawer',
						"A custom app for balance input
						
						Developed by Joseph Lane using Ruby and Glimmer
						Cash Drawer - \u{00A9} 2024")
				end
			}
		}
	end
	
	def launch
	
		file_menu
		help_menu
	
		window('Cash Drawer', 700, 455, has_menubar = true) {
			margined true
			resizable false
			
			tab {
				tab_item('Funds') {
				
					vertical_box {
					
						grid {
							
							# Currency label fields
							label('Date') {
								left 0
								top 0
								halign :left
								valign :center
							}
							
							label('Pennies') {
								left 0
								top 1
								halign :left
								valign :center
							}
							
							label('Nickels') {
								left 0
								top 2
								halign :left
								valign :center
							}
							
							label('Dimes') {
								left 0
								top 3
								halign :left
								valign :center
							}
							
							label('Quarters') {
								left 0
								top 4
								halign :left
								valign :center
							}
							
							label('Ones') {
								left 0
								top 5
								halign :left
								valign :center
							}
							
							label('Fives') {
								left 0
								top 6
								halign :left
								valign :center
							}
							
							label('Tens') {
								left 0
								top 7
								halign :left
								valign :center
							}
							
							label('Twentys') {
								left 0
								top 8
								halign :left
								valign :center
							}
							
							label('Fiftys') {
								left 0
								top 9
								halign :left
								valign :center
							}
							
							label('Hundreds') {
								left 0
								top 10
								halign :left
								valign :center
							}
							
							date_picker {
								left 1
								top 0
								hexpand true unless OS.mac?
								time <=> [self, :date]
								@date = "#{Time.now.year}-#{'%02d' % Time.now.month}-#{'%02d' % Time.now.day}"
								on_changed do
									@date = "#{date[:year]}-#{'%02d' % date[:mon]}-#{'%02d' % date[:mday]}"
								end
							}
							
							# Currency entry fields
							entry {
								left 1
								top 1
								hexpand true unless OS.mac?
								@penny = '0'
								text <=> [self, :penny]
							}
							
							entry {
								left 1
								top 2
								@nickel = '0'
								text <=> [self, :nickel]
							}
							
							entry {
								left 1
								top 3
								@dime = '0'
								text <=> [self, :dime]
							}
							
							entry {
								left 1
								top 4
								@quarter = '0'
								text <=> [self, :quarter]
							}
							
							entry {
								left 1
								top 5
								@ones = '0'
								text <=> [self, :ones]
							}
							
							entry {
								left 1
								top 6
								@fives = '0'
								text <=> [self, :fives]
							}
							
							entry {
								left 1
								top 7
								@tens = '0'
								text <=> [self, :tens]
							}
							
							entry {
								left 1
								top 8
								@twentys = '0'
								text <=> [self, :twentys]
							}
							
							entry {
								left 1
								top 9
								@fiftys = '0'
								text <=> [self, :fiftys]
							}
							
							entry {
								left 1
								top 10
								@bennys = '0'
								text <=> [self, :bennys]
							}
							
							# Accessory label fields
							
							label('Checks') {
								left 2
								top 1
								halign :left
								valign :center
							}
							
							label('Bank Account') {
								left 2
								top 2
								halign :left
								valign :center
							}
							
							label('Motors Due') {
								left 2
								top 3
								halign :left
								valign :center
							}
							
							label('Petty Cash') {
								left 2
								top 4
								halign :left
								valign :center
							}
							
							label('Cash Bag') {
								left 2
								top 5
								halign :left
								valign :center
							}
							
							label('Office') {
								left 2
								top 6
								halign :left
								valign :center
							}
							
							label('Copy Monies') {
								left 2
								top 7
								halign :left
								valign :center
							}
							
							label('Motors') {
								left 2
								top 8
								halign :left
								valign :center
							}
							
							label('Firearms') {
								left 2
								top 9
								halign :left
								valign :center
							}
							
							label('Fingerprints') {
								left 2
								top 10
								halign :left
								valign :center
							}
							
							# Accessory entry fields
							entry {
								left 3
								top 1
								hexpand true unless OS.mac?
								@checks = '0'
								text <=> [self, :checks]
							}
							
							entry {
								left 3
								top 2
								@bank = '0'
								text <=> [self, :bank]
							}
							
							entry {
								left 3
								top 3
								@mvdue = '0'
								text <=> [self, :mvdue]
							}
							
							entry {
								left 3
								top 4
								@petty = '0'
								text <=> [self, :petty]
							}
							
							entry {
								left 3
								top 5
								@cbag = '0'
								text <=> [self, :cbag]
							}
							
							entry {
								left 3
								top 6
								@office = '0'
								text <=> [self, :office]
							}
							
							entry {
								left 3
								top 7
								@copy = '0'
								text <=> [self, :copy]
							}
							
							entry {
								left 3
								top 8
								@motors = '0'
								text <=> [self, :motors]
							}
							
							entry {
								left 3
								top 9
								@guns = '0'
								text <=> [self, :guns]
							}
							
							entry {
								left 3
								top 10
								@prints = '0'
								text <=> [self, :prints]
							}
							
							# Submit button
							button('Submit') {
								left 3
								top 11
								
								on_clicked do
									@dtotal = 	BigDecimal(@penny.tr('^0-9.', '')) +
															BigDecimal(@nickel.tr('^0-9.', '')) +
															BigDecimal(@dime.tr('^0-9.', '')) +
															BigDecimal(@quarter.tr('^0-9.', '')) +
															BigDecimal(@ones.tr('^0-9.', '')) +
															BigDecimal(@fives.tr('^0-9.', '')) + 
															BigDecimal(@tens.tr('^0-9.', '')) +
															BigDecimal(@twentys.tr('^0-9.', '')) + 
															BigDecimal(@fiftys.tr('^0-9.', '')) +
															BigDecimal(@bennys.tr('^0-9.', ''))
										
									@onhand = 	BigDecimal(@checks.tr('^0-9.', '')) +
															BigDecimal(@bank.tr('^0-9.', '')) +
															BigDecimal(@mvdue.tr('^0-9.', '')) +
															BigDecimal(@petty.tr('^0-9.', '')) +
															BigDecimal(@cbag.tr('^0-9.', ''))
										
									@payable = 	BigDecimal(@office.tr('^0-9.', '')) +
															BigDecimal(@petty.tr('^0-9.', '')) +
															BigDecimal(@copy.tr('^0-9.', '')) +
															BigDecimal(@motors.tr('^0-9.', '')) +
															BigDecimal(@guns.tr('^0-9.', '')) +
															BigDecimal(@prints.tr('^0-9.', ''))
										
									@diff = BigDecimal(@onhand) + BigDecimal(@dtotal) - BigDecimal(@payable)
							
									Total.new(@date, Money.from_amount(0), Money.from_amount(0), Money.from_amount(0), Money.from_amount(0))
									@totals << Total.new(@date, Money.from_amount(@dtotal).format, Money.from_amount(@onhand).format,
										Money.from_amount(@payable).format, Money.from_amount(@diff).format)
									msg_box('Notice', 'Values have been added to the ledger.')
								end
							}
						}
					}
				}
				
				tab_item('Summary'){
					table {
						text_column('Date') {editable true}
						text_column('Dtotal')
						text_column('Onhand')
						text_column('Payable')
						text_column('Diff')

						cell_rows <=> [self, :totals]
					}
				}
			}	
		}.show
	end
end

CashDrawer.new.launch
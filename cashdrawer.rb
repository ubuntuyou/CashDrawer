require 'glimmer-dsl-libui'
require 'time'

class CashDrawer

	include Glimmer

	Funds = Struct.new(
		:ones,
		:fives,
		:tens,
		:twentys,
		:fiftys,
		:bennys,
		:checks,
		:petty
	)
	
	Total = Struct.new( :date, :dtotal, :onhand, :ap )
	
	attr_accessor :totals
	attr_accessor :ones, :fives, :tens, :twentys, :fiftys, :bennys
	attr_accessor :penny, :nickel, :dime, :quarter
	attr_accessor :checks, :petty, :bank, :mvdue, :cbag
	attr_accessor :date, :dtotal, :onhand, :ap, :dt

	def initialize
		@totals = [
			Total.new('2024-11-23', '$17.99', '$534.88', '$18.37')
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
	
		window('Cash Drawer', 400, 300, has_menubar = true) {
			margined true
			tab {
				tab_item('Funds') {
				
					vertical_box {
						horizontal_box {
						
							group {
							stretchy false
								vertical_box {
									form {
										stretchy false
										
										date_picker {
											time <=> [self, :date]
											@date = "#{Time.now.year}-#{Time.now.month}-#{Time.now.day}"
											p @date
											on_changed do
												@date = "#{date[:year]}-#{date[:mon]}-#{date[:mday]}"
												p @date
											end
										}
										
										entry {
											label 'Pennies'
											text <=> [self, :penny]
										}
										
										entry {
											label 'Nickels'
											text <=> [self, :nickel]
										}
										
										entry {
											label 'Dimes'
											text <=> [self, :dime]
										}
										
										entry {
											label 'Quarters'
											text <=> [self, :quarter]
										}
									}	
								}
							}
							
							group{
							stretchy false
								vertical_box {
									form {
										stretchy false
										
										entry {
											label 'Ones'
											text <=> [self, :ones]
										}
										
										entry {
											label 'Fives'
											text <=> [self, :fives]
										}
										
										entry {
											label 'Tens'
											text <=> [self, :tens]
										}
										
										entry {
											label 'Twentys'
											text <=> [self, :twentys]
										}
										
										entry {
											label 'Fiftys'
											text <=> [self, :fiftys]
										}
										
										entry {
											label 'Hundreds'
											text <=> [self, :bennys]
										}
									}	
								}
							}

							group {
							stretchy false
								vertical_box {
									form {
										stretchy false
										
										entry {
											label 'Checks'
											text <=> [self, :checks]
										}
										
										entry {
											label 'Petty Cash'
											text <=> [self, :petty]
										}
										
										entry {
											label 'On Hand'
											text <=> [self, :onhand]
										}
									}
								
								}
							}
						}
						
						group {
						stretchy false
							vertical_box {
								form {
									stretchy false
									button('Submit') {
										stretchy false
										
										on_clicked do
											@totals << Total.new(@date, @dtotal, @onhand, @ap)
											msg_box('Notice', 'Values have been added to the ledger.')
										end
									}
								}
							}
						}
					}
				}
				
				tab_item('Summary'){
					table {
						text_column('Date')
						text_column('Dtotal')
						text_column('Onhand')
						text_column('Ap')

						cell_rows <=> [self, :totals]
					}
				}
			}	
		}.show
	end
end

CashDrawer.new.launch
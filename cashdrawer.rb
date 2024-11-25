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
	
	Total = Struct.new( :date, :dtotal, :onhand, :ap )
	
	attr_accessor :totals
	attr_accessor :ones, :fives, :tens, :twentys, :fiftys, :bennys
	attr_accessor :penny, :nickel, :dime, :quarter
	attr_accessor :checks, :petty, :bank, :mvdue, :cbag
	attr_accessor :office, :copy, :motors, :guns, :prints
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
											label 'Date'
											time <=> [self, :date]
											@date = "#{Time.now.year}-#{'%02d' % Time.now.month}-#{'%02d' % Time.now.day}"
											on_changed do
												@date = "#{date[:year]}-#{'%02d' % date[:mon]}-#{'%02d' % date[:mday]}"
											end
										}
										
										
										entry {
											@penny = '0'
											label 'Pennies'
											text <=> [self, :penny]
										}
										
										entry {
											@nickel = '0'
											label 'Nickels'
											text <=> [self, :nickel]
										}
										
										entry {
											@dime = '0'
											label 'Dimes'
											text <=> [self, :dime]
										}
										
										entry {
											@quarter = '0'
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
											@ones = '0'
											label 'Ones'
											text <=> [self, :ones]
										}
										
										entry {
											@fives = '0'
											label 'Fives'
											text <=> [self, :fives]
										}
										
										entry {
											@tens = '0'
											label 'Tens'
											text <=> [self, :tens]
										}
										
										entry {
											@twentys = '0'
											label 'Twentys'
											text <=> [self, :twentys]
										}
										
										entry {
											@fiftys = '0'
											label 'Fiftys'
											text <=> [self, :fiftys]
										}
										
										entry {
											@bennys = '0'
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
											@checks = '0'
											label 'Checks'
											text <=> [self, :checks]
										}
										
										entry {
											@bank = '0'
											label 'Bank'
											text <=> [self, :bank]
										}
										
										entry {
											@mvdue = '0'
											label 'MV Due'
											text <=> [self, :mvdue]
										}
										
										entry {
											@petty = '0'
											label 'Petty Cash'
											text <=> [self, :petty]
										}
										
										entry {
											@cbag = '0'
											label 'Cash Bag'
											text <=> [self, :cbag]
										}
									}
								}
							}
						}
						horizontal_box {
							stretchy false
							
							group {
								stretchy false
							vertical_box {
								form {
									stretchy false
									
									entry {
										@office = '0'
										label 'Office'
										text <=> [self, :office]
									}
									
									entry {
										@copy = '0'
										label 'Copy'
										text <=> [self, :copy]
									}
									
									entry {
										@motors = '0'
										label 'Motors'
										text <=> [self, :motors]
									}
									
									entry {
										@guns = '0'
										label 'Firearms'
										text <=> [self, :guns]
									}
									
									entry {
										@prints = '0'
										label 'Prints'
										text <=> [self, :prints]
									}
								}
							}
							}
							
							vertical_box {
								form {
									stretchy false
								}
							}
							
							
							
							vertical_box {
								form {
									stretchy false
								}
							
							}
						}
						
						#group {
						#	stretchy false
							
						#	vertical_box {
								
							
								horizontal_box {
									#form {
										stretchy false
										
										button('Calculate') {
											stretchy false
											
											on_clicked do
												@dtotal = BigDecimal(@penny) + BigDecimal(@nickel) + 
													BigDecimal(@dime) + BigDecimal(@quarter) +
													BigDecimal(@ones) + BigDecimal(@fives) + 
													BigDecimal(@tens) + BigDecimal(@twentys) + 
													BigDecimal(@fiftys) + BigDecimal(@bennys)
													
												@onhand = BigDecimal(@checks) + BigDecimal(@bank) +
													BigDecimal(@mvdue) + BigDecimal(@petty) + BigDecimal(@cbag)
													
												@ap = 
												
												p Money.from_amount(@onhand).format
												p Money.from_amount(@dtotal).format
												p Money.from_amount(@ap).format
											end
										}
										
										button('Submit') {
											stretchy false
											
											on_clicked do
												if @onhand.nil? then @onhand = "0.00" end
												if @dtotal.nil? then @dtotal = "0.00" end
												if @ap.nil? then @ap = "0.00" end
												
												@totals << Total.new(@date, Money.from_amount(@dtotal).format, Money.from_amount(@onhand).format, @ap)
												msg_box('Notice', 'Values have been added to the ledger.')
											end
										}
									#}
								}
							#}
						#}
					}
				}
				
				tab_item('Summary'){
					table {
						text_column('Date') {editable true}
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
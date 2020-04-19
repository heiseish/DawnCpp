#pragma once

#ifndef DAWN_TRADING_STRAT_META_INDICATOR_H
#define DAWN_TRADING_STRAT_META_INDICATOR_H

#include <string_view>
#include <vector>
namespace Dawn::Trading {

class MetaIndicator(meta.linesholder.LinesHolder.__class__):
    # The metaclass takes care of parsing the appropriate defintions during
    # class creation (alias, lines, ...) and properly definiing them if needed
    # in the final class (considering the bases)

    def __new__(metacls, name, bases, dct, **kwargs):
        # Creates class, gathers all lines definitions, installs aliases and
        # auto-exports itself and the aliases
        cls = super().__new__(metacls, name, bases, dct)  # create

        meta.aliases._generate(cls, bases, dct, **kwargs)
        meta.inputs._generate(cls, bases, dct, **kwargs)
        meta.outputs._generate(cls, bases, dct, **kwargs)
        meta.params._generate(cls, bases, dct, **kwargs)
        meta.docs._generate(cls, bases, dct, **kwargs)
        meta.groups._generate(cls, bases, dct, **kwargs)

        to_register = cls.__module__.split('.')[0] == __package__
        if to_register and name != 'Indicator' and not name.startswith('_'):
            _NAMES_IND[name] = cls
            _IND_NAMES[cls] = name

            for grp in cls.group:
                _GRP_IND[grp].append(cls)

        return cls  # return newly created and patched class

    def __call__(cls, *args, **kwargs):
        # In charge of object creation and initialization.
        # Parses and assigns declared parameters
        # Adds auto-magical
        # member attributes before __init__ is given a change to do
        # something. Any subclass with something to do in __init__ will already
        # be able to access the auto-magical attributes
        self = cls.__new__(cls, *args, *kwargs)  # create instance as usual

        # Create and install the lines holding instance
        self.outputs = self.o = meta.outputs._from_class(cls)
        self.lines = self.l = self.outputs  # noqa: E741

        # Get inputs and remaining args
        self.inputs, args = meta.inputs._from_args(cls, *args)
        self.i = self.inputs  # shorthand

        # Add array of data feeds ... the s in "datas" to indicate multiple
        self.datas = self.d = list(self.inputs)
        self.data = self.datas[0]  # add main alias to 1st data

        # add direct aliases with numeric index
        for i, _in in enumerate(self.inputs):
            for inalias in ('i{}', 'input{}', 'd{}', 'data{}'):
                setattr(self, inalias.format(i), _in)

        # add direct aliases with naming index
        for i, _in in enumerate(self.inputs.__slots__):
            for inalias in ('i_{}', 'input_{}', 'd_{}', 'data_{}'):
                setattr(self, inalias.format(i), _in)

        # Gather minimum periods and get the dominant mininum period
        self._minperiods = [_in._minperiod for _in in self.inputs]
        self._minperiod = max(self._minperiods)

        # Check if ta-lib compatibility is requested. If so and the indicator
        # defines a _talib function, give it the **ACTUAL** kwargs and use the
        # modified version. Don't let a '_talib' parameter make it to the
        # indicator (hence pop)
        if config.get_talib_compat() or kwargs.pop('_talib', False):
            getattr(self, '_talib')(kwargs)  # actual kwargs passed

        # Get params instance and remaining kwargs
        self.params, kwargs = meta.params._from_kwargs(cls, **kwargs)
        self.p = self.params  # shorthand

        # All boilerplate is done, to into execution mode
        metadata.callstack.append(self)  # let ind know hwere in the stack

        if cls._autosuper:
            super(cls, self).__init__(*args, **kwargs)  # auto-run bases

        self.__init__(*args, **kwargs)  # initialize the instance

        # delete old aliases only meant for operational purposes
        for oalias in ('l', 'lines', 'data', 'd', 'datas'):
            delattr(self, oalias)

        # remove direct aliases with numeric index
        for i, _in in enumerate(self.inputs):
            for inalias in ('d{}', 'data{}'):
                delattr(self, inalias.format(i))

        # remove direct aliases with naming index
        for i, _in in enumerate(self.inputs.__slots__):
            for inalias in ('d_{}', 'data_{}'):
                delattr(self, inalias.format(i))

        # update min periods of lines after calculations and replicate
        self.outputs._update_minperiod()
        self._minperiods = self.outputs._minperiods
        self._minperiod = self.outputs._minperiod

        metadata.callstack.pop()  # let ind know hwere in the stack

        # set def return value, but consider stack depth and user pref
        ret = self
        if not metadata.callstack:  # top-of the stack, ret following prefs
            if config.get_return_dataframe():
                ret = self.df

        return ret  # Return itself for now


}

#endif